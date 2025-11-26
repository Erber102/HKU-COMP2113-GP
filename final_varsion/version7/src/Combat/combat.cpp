#include "combat.h"
#include "../Core/InputSystem.h"
#include "../Player/Player.h"
#include "../Item/item.h"
#include<iostream>
#include<memory>
#include<cstdlib>
#include<ctime>
using namespace std;
//Initializing the player and enemies
Combat::Combat(Player* owner, const string &ply, Item* startingWeapon)
:currentEnemyIndex(0),score(0),playerRef(owner){
    srand(time(0));
    player.reset(new CombatPlayer(ply));
    setCurrentWeapon(startingWeapon);
    player->setHealingCallback([this]() { return useHealingItem(); });
    player->setAmmoCallback([this]() { return consumeAmmoForCurrentWeapon(); });
    player->setBackpackCallback([this]() { openWeaponMenu(); });
    player->setWeaponUsageCallback([this]() { handleWeaponDurability(); });
    enemies.push_back(std::unique_ptr<Enemy>(new Enemy("Noise Monster","Rhythm Breaker",60)));
    enemies.push_back(std::unique_ptr<Enemy>(new Enemy("Melody Thief","Melody Thief",70)));
    enemies.push_back(std::unique_ptr<Enemy>(new Enemy("Harmony Monster","Harmony Monster",100)));
    enemies.push_back(std::unique_ptr<Enemy>(new Enemy("Final Symphony","Rhythm Breaker",120)));
    InputSystem::setNonBlockingInput();
}
//restoring the setting of the terminal
Combat::~Combat(){
    InputSystem::restoreTerminal();
}
//use the index to distinguish the enemy the player is dealing with
void Combat::fightEnemy(int enemyIndex){
    Enemy &enemy=*enemies[enemyIndex];
    //show the name of encountered enemy
    InputSystem::clearScreen();
    cout<<BOLD<<"\nYour encountered "<<RED<<enemy.getName()<<"!"<<RESET<<endl;
    showRuneIntro();
    InputSystem::sleepMs(3000);
    //start the turns
    while(player->isAlive()&&enemy.isAlive()){
        player->resetTurn();
        player->takeTurn(enemy);
        if(!enemy.isAlive())break;
        while(player->hasExtraTurns()&&enemy.isAlive()){
            player->useExtraTurn();
            cout<<BOLD<<GREEN<<"\n Extra turns!"<<RESET<<endl;
            
            player->takeTurn(enemy);
            if(!enemy.isAlive())break;
        }
        enemy.resetTurn();
        enemy.takeTurn(*player);
    }
    //show ending titles, more functions could be implemented in the showVictory() and showGameOver()
    InputSystem::clearScreen();
    if(player->isAlive()){
        showVictory();
        int reward = calculateReward(enemyIndex);
        if(playerRef && reward > 0){
            playerRef->money += reward;
            cout<<GREEN<<"You looted "<<reward<<" Money from the battle!"<<RESET<<endl;
        }
    }
    else showGameOver();
}

void Combat::equipWeapon(Item* weaponPtr){
    setCurrentWeapon(weaponPtr);
}

int Combat::useHealingItem(){
    if(!playerRef) return 0;
    vector<Item*>& inv = playerRef->getInventory();
    Item* best = nullptr;
    size_t bestIndex = 0;
    for(size_t i=0;i<inv.size();++i){
        Item* item = inv[i];
        if(item && item->healthEffect > 0){
            if(!best || item->healthEffect > best->healthEffect){
                best = item;
                bestIndex = i;
            }
        }
    }
    if(!best) return 0;
    string itemName = best->name;
    int healAmount = best->healthEffect;
    playerRef->removeItem(itemName);
    cout<<CYAN<<"Consumed "<<itemName<<" for healing."<<RESET<<endl;
    return healAmount;
}

bool Combat::consumeAmmoForCurrentWeapon(){
    if(!currentWeapon.needsAmmo) return true;
    if(!playerRef) return false;
    std::string ammo = currentWeapon.ammoItem.empty() ? "Bullet" : currentWeapon.ammoItem;
    if(ammo.empty()) return true;
    if(playerRef->hasItem(ammo)){
        std::string ammoName = ammo;
        playerRef->removeItem(ammoName);
        cout<<YELLOW<<"Consumed "<<ammo<<" for "<<currentWeapon.name<<"."<<RESET<<endl;
        return true;
    }
    cout<<RED<<"No "<<ammo<<" left! "<<currentWeapon.name<<" deals no extra damage."<<RESET<<endl;
    InputSystem::waitForAnyKey();
    return false;
}

void Combat::openWeaponMenu(){
    if(!playerRef){
        cout<<RED<<"Cannot access backpack right now."<<RESET<<endl;
        InputSystem::waitForAnyKey();
        return;
    }
    vector<Item*>& inv = playerRef->getInventory();
    vector<Item*> weapons;
    for(Item* item : inv){
        if(item && item->category == CATEGORY_WEAPON){
            weapons.push_back(item);
        }
    }
    InputSystem::clearScreen();
    InputSystem::drawTitle("Backpack - Weapons");
    if(weapons.empty()){
        cout<<YELLOW<<"No weapons available. Using bare hands."<<RESET<<endl;
        setCurrentWeapon(nullptr);
        InputSystem::waitForAnyKey();
        return;
    }
    bool selecting=true;
    while(selecting){
        InputSystem::clearScreen();
        InputSystem::drawTitle("Select Weapon");
        for(size_t i=0;i<weapons.size();++i){
            Item* item = weapons[i];
            bool needsAmmo = (item->name == "Shotgun");
            cout<<"["<<(i+1)<<"] "<<item->name<<" - Damage "<<item->damage
                <<" | Durability "<<item->durability;
            if(needsAmmo) cout<<" (requires Bullets)";
            cout<<endl;
        }
        cout<<"[0] Cancel"<<endl;
        cout<<YELLOW<<"Press number to equip weapon."<<RESET<<endl;
        bool gotInput=false;
        while(!gotInput){
            if(InputSystem::kbhit()){
                char key=InputSystem::getch();
                if(key=='0'){
                    cout<<YELLOW<<"Weapon selection cancelled."<<RESET<<endl;
                    InputSystem::waitForAnyKey();
                    return;
                }
                if(key>='1' && key<='9'){
                    int option = key-'0';
                    if(option>=1 && option <= static_cast<int>(weapons.size())){
                        Item* chosen = weapons[option-1];
                        setCurrentWeapon(chosen);
                        cout<<GREEN<<"Equipped "<<currentWeapon.name<<" (+"
                            <<currentWeapon.damage<<" dmg";
                        if(currentWeapon.needsAmmo) cout<<", uses Bullets";
                        cout<<")."<<RESET<<endl;
                        InputSystem::waitForAnyKey();
                        return;
                    }
                }
                cout<<RED<<"Invalid selection."<<RESET<<endl;
                InputSystem::waitForAnyKey();
                gotInput = true;
            }
            InputSystem::sleepMs(100);
        }
    }
}

void Combat::setCurrentWeapon(Item* weaponPtr){
    currentWeapon.itemPtr = weaponPtr;
    applyEquippedWeapon();
}

void Combat::applyEquippedWeapon(){
    if(currentWeapon.itemPtr){
        currentWeapon.name = currentWeapon.itemPtr->name;
        currentWeapon.damage = currentWeapon.itemPtr->damage;
        currentWeapon.needsAmmo = (currentWeapon.name == "Shotgun");
        currentWeapon.ammoItem = currentWeapon.needsAmmo ? "Bullet" : "";
    } else {
        currentWeapon.name = "Bare Hands";
        currentWeapon.damage = 0;
        currentWeapon.needsAmmo = false;
        currentWeapon.ammoItem.clear();
    }
    if(player){
        int durability = currentWeapon.itemPtr ? currentWeapon.itemPtr->durability : -1;
        player->setWeaponBonus(currentWeapon.damage, currentWeapon.name, currentWeapon.needsAmmo, durability);
    }
}
void Combat::handleWeaponDurability(){
    if(!playerRef) return;
    if(!currentWeapon.itemPtr) return;
    currentWeapon.itemPtr->durability -= 5;
    if(currentWeapon.itemPtr->durability <= 0){
        std::string brokenName = currentWeapon.itemPtr->name;
        playerRef->removeSpecificItem(currentWeapon.itemPtr);
        cout<<RED<<brokenName<<" broke!"<<RESET<<endl;
        setCurrentWeapon(nullptr);
        InputSystem::waitForAnyKey();
    } else {
        applyEquippedWeapon();
    }
}

int Combat::calculateReward(int enemyIndex) const{
    static const int rewards[] = {15, 30, 45, 70};
    if(enemyIndex < 0) return 0;
    int cappedIndex = enemyIndex;
    if(cappedIndex >= static_cast<int>(sizeof(rewards)/sizeof(rewards[0]))){
        cappedIndex = sizeof(rewards)/sizeof(rewards[0]) - 1;
    }
    return rewards[cappedIndex];
}
//intro panel to the rune
void Combat::showRuneIntro(){
    cout<<BOLD<<MAGENTA;
    InputSystem::drawSeparator();
    cout<<RESET;
    cout<<"Use the number keys 1-7 to play notes, and space to activate runes."<<endl;
    cout<<endl;
    cout<<BOLD<<"Rune combinations:"<<RESET<<endl;
    cout<<GREEN<<"1-2-1 Cure Rune"<<RESET<<endl;
    cout<<RED<<"3-4-3 Attack Rune"<<RESET<<endl;
    cout<<BLUE<<"5-6-5 Defense Rune"<<RESET<<endl;
}
//showing the panel for victory and Go
void Combat::showVictory(){
    InputSystem::clearScreen();
    cout<<BOLD<<GREEN;
    InputSystem::drawTitle("Congratulations! Your have defeated the enemy!");
    InputSystem::sleepMs(2000);
    cout<<RESET;
}
void Combat::showGameOver(){
    InputSystem::clearScreen();
    cout<<BOLD<<RED;
    InputSystem::drawTitle("You failed to defeat the enemy.");
    InputSystem::sleepMs(2000);
    cout<<RESET;
}