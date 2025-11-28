#include "CombatPlayer.h"
#include "../Core/InputSystem.h"
#include "utils.h"
#include<iostream>
#include<memory>
#include<algorithm>
using namespace std;
//initiate combat settings of player, runes, and note effects
//name: string name representing the player
CombatPlayer::CombatPlayer(const string &name)
:Character(name,100),harmony(5),combo(0),extraTurns(0),
 weaponBonusDamage(0),equippedWeaponName("Bare Hands"),
 weaponNeedsAmmo(false),weaponDurability(-1){
    addRune(std::unique_ptr<Rune>(new Rune("Cure Rune",vector<Note>{DO,RE,DO},"Heal",20,GREEN)));
    addRune(std::unique_ptr<Rune>(new Rune("Attack Rune",vector<Note>{MI,FA,MI},"Damage",25,RED)));
    addRune(std::unique_ptr<Rune>(new Rune("Defense Rune",vector<Note>{SOL,LA,SOL},"Defense",15,BLUE)));
    addNoteEffect(DO,std::unique_ptr<NoteEffect>(new NoteEffect("Heal","Recover small amount of health",8,GREEN)));
    addNoteEffect(RE,std::unique_ptr<NoteEffect>(new NoteEffect("Power up","Recover resonance",10,BLUE)));
    addNoteEffect(MI,std::unique_ptr<NoteEffect>(new NoteEffect("Attack","Damage the enemy",12,RED)));
    addNoteEffect(FA,std::unique_ptr<NoteEffect>(new NoteEffect("Shield","aquire temper defense",5,CYAN)));
    addNoteEffect(SOL,std::unique_ptr<NoteEffect>(new NoteEffect("Purge","Rurge and recover small amount of health",5,MAGENTA)));
    addNoteEffect(LA,std::unique_ptr<NoteEffect>(new NoteEffect("Variation","Random effect",0,WHITE)));
    addNoteEffect(SI,std::unique_ptr<NoteEffect>(new NoteEffect("Accumulation","Extra turns",0,WHITE)));
}

// Configures weapon bonuses and properties for the combat player
// bonus - Integer damage bonus provided by the weapon
// weaponName - String name of the equipped weapon
// needsAmmo - Boolean indicating if weapon requires ammunition
// durability - Integer representing weapon durability (-1 for infinite)
// return void - No return value, updates weapon properties
void CombatPlayer::setWeaponBonus(int bonus, const std::string& weaponName, bool needsAmmo, int durability){
    weaponBonusDamage = bonus;
    weaponNeedsAmmo = needsAmmo;
    equippedWeaponName = weaponName.empty() ? "Bare Hands" : weaponName;
    weaponDurability = durability;
}

// Sets callback function for consuming healing items during combat
// func - Function pointer for healing item consumption
void CombatPlayer::setHealingCallback(std::function<int()> func){
    consumeHealingItemFunc = func;
}

// Sets callback function for ammunition consumption during combat
// func - Function pointer for ammo consumption check
void CombatPlayer::setAmmoCallback(std::function<bool()> func){
    consumeAmmoCallback = func;
}

// Sets the callback function for opening backpack during combat
// This enables backpack functionality integration into the combat phase
// func - Function pointer/callback for backpack opening
void CombatPlayer::setBackpackCallback(std::function<void()> func){
    openBackpackCallback = func;
}

// Sets callback function for weapon usage tracking
// func - Function pointer for weapon usage events
void CombatPlayer::setWeaponUsageCallback(std::function<void()> func){
    weaponUsageCallback = func;
}
//increase harmony by 1
void CombatPlayer::increaseHarmony(){
    harmony=min(10,harmony+1);
}
//increase combo by 1
void CombatPlayer::increaseCombo(){
    combo++;
}
//reset the combo to 0
void CombatPlayer::resetCombo(){
    combo=0;
}
//return the value of combo
int CombatPlayer::getCombo(){
    return combo;
}
//reture the value of harmony
int CombatPlayer::getHarmony(){
    return harmony;
}
//check whether the player has extra turns 
bool CombatPlayer::hasExtraTurns(){
    return extraTurns>0;
}
//use the extra turn
void CombatPlayer::useExtraTurn(){
    if(extraTurns>0)extraTurns--;
}
//check if any rune is activated
//target: enemy the rune is applied on 
//return 1 for activated, 0 or not activated
bool CombatPlayer::activateRune(Character &target){
    int activeRuneIndex=checkMelody();
    if(activeRuneIndex==-1)return 0;
    const auto &activeRune=runes[activeRuneIndex];
    cout<<BOLD<<activeRune->color<<" !Detecting active rune: "<<activeRune->name<<"! "<<RESET<<endl;
    cout<<"Effect: "<<activeRune->effect<<endl;
    cout<<YELLOW<<"Press "<<RED<<"[Space]"<<YELLOW<<" to activate rune, or press other keys to continue..."<<RESET<<endl;
    while(1){
        if(InputSystem::kbhit()){
            char key=InputSystem::getch();
            if(key==' '){
                CombatPlayer::activeRune(activeRuneIndex,target);
                return 1;
            }
            else {
                InputSystem::waitForAnyKey();
                break;
            }
        }
        InputSystem::sleepMs(100);
    }
    return 0;
}

//flow of player's turn, target represents the enemy
//target - Reference to enemy Character being fought
void CombatPlayer::takeTurn(Character &target){
    bool actionTaken = false;
    while(!actionTaken){
        //information display
        InputSystem::clearScreen();
        InputSystem::drawTitle("Your Turn");
        cout<<endl<<"Your health:    "<<getHealthBar()<<endl;
        cout<<"Enemy's health: "<<target.getHealthBar()<<endl<<endl;
        cout<<endl;
        cout<<"Resonance: "<<getResonanceBar();
        cout<<" Harmony: "<<YELLOW<<harmony<<"/10"<<RESET;
        cout<<" | Combo: "<<CYAN<<combo<<RESET;
        if(extraTurns>0){
            cout<<" | extra turn: "<<GREEN<< extraTurns<<RESET;
        }
        cout<<endl<<"Weapon: "<<WHITE<<equippedWeaponName<<RESET;
        if(weaponBonusDamage > 0){
            cout<<" (+"<<weaponBonusDamage<<" dmg";
            if(weaponNeedsAmmo) cout<<", uses ammo";
            if(weaponDurability >= 0) cout<<", dur "<<weaponDurability;
            cout<<")";
        } else {
            cout<<" (bare hands)";
        }
        cout<<endl;
        cout<<getMelodyDisplay()<<endl<<endl;
        showNoteInputMenu();
        if(activateRune(target))return ;
        //wait for further input
        cout<<YELLOW<<"Please select an operation to continue..."<<RESET<<endl;
        bool inputHandled = false;
        while(!inputHandled){
            if(InputSystem::kbhit()){
                char key=InputSystem::getch();
                bool consumed = handleInput(key,target);
                if(consumed){
                    changeResonance(5);
                    actionTaken = true;
                }
                inputHandled = true;
            }
            InputSystem::sleepMs(100);
        }
    }
}
//function for displaying the simplified effects of the notes
void CombatPlayer::showNoteInputMenu(){
    InputSystem::drawSeparator();
    cout<<BOLD<<"Play note: "<<RESET<<endl;
    //for instructions
    cout<<" [1] DO  - "<<GREEN<<"Heal"<<RESET<<endl;
    cout<<" [2] RE  - "<<BLUE<<"Power up"<<RESET<<endl;
    cout<<" [3] MI  - "<<RED<<"Attack"<<RESET<<endl;
    cout<<" [4] FA  - "<<CYAN<<"Shield"<<RESET<<endl;
    cout<<" [5] SOL - "<<YELLOW<<"Purge"<<RESET<<endl;
    cout<<" [6] LA  - "<<MAGENTA<<"Variation"<<RESET<<endl;
    cout<<" [7] SI  - "<<WHITE<<"Extra turns"<<RESET<<endl;
    cout<<" [B] Open backpack (switch weapon)"<<endl; // Display backpack option and allows accessing inventory during combat
    cout<<endl;
}
//relate the keys to the effect functions
//key: related key
//target: the target the effect is applied on
//returns 1 if the input is correctly handed, else 0
bool CombatPlayer::handleInput(char key,Character &target){
    bool notePlayed=1;
    switch(key){
        case '1':
            addNoteToMelody(DO);
            applyNoteEffect(DO,target);
            break;
        case '2':
            addNoteToMelody(RE);
            applyNoteEffect(RE,target);
            break;
        case '3':
            addNoteToMelody(MI);
            applyNoteEffect(MI,target);
            break;
        case '4':
            addNoteToMelody(FA);
            applyNoteEffect(FA,target);
            break;
        case '5':
            addNoteToMelody(SOL);
            applyNoteEffect(SOL,target);
            break;
        case '6':
            addNoteToMelody(LA);
            applyNoteEffect(LA,target);
            break;
        case '7':
            addNoteToMelody(SI);
            applyNoteEffect(SI,target);
            break;
        case 'b':
        case 'B':
            // Player can press 'B' to open backpack and switch weapons
            if(openBackpackCallback){
                openBackpackCallback();
            } else {
                cout<<RED<<"Backpack unavailable."<<RESET<<endl;
                InputSystem::waitForAnyKey();
            }
            return false;
        default:
            cout<<RED<<"Invalid input! Please use 1-7 to play note"<<RESET<<endl;
            notePlayed=0;
    }
    if(notePlayed){
        increaseCombo();
        if(combo%5==0){
            cout<<BOLD<<CYAN<<"Combo "<<combo<<"! Resonance increased !"<<RESET<<endl;
            changeResonance(combo);
        }
        activateRune(target);
        if(key!=' ')InputSystem::waitForAnyKey();
        return true;
    }
    if(key!='b' && key!='B'){
        while(1){
            if(InputSystem::kbhit()){
                char key=InputSystem::getch();
                handleInput(key,target);
                break;
            }
            InputSystem::sleepMs(100);
        }
    }
    return false;
}
//applying the specific note effect
//note: is the note applied
//target: the enemy
void CombatPlayer::applyNoteEffect(Note note,Character &target){
    const NoteEffect *effect=getNoteEffect(note);
    if(!effect)return ;
    cout<<BOLD<<effect->color<<effect->name<<"!"<<RESET<<endl;
    switch(note){
        case DO:
            {
            int itemHeal = consumeHealingItemFunc ? consumeHealingItemFunc() : 0;
            if(itemHeal <= 0){
                cout<<RED<<"No medical supplies available! Heal failed."<<RESET<<endl;
                break;
            }
            int healAmount=itemHeal;
            heal(healAmount);
            cout<<GREEN<<"Used medical supplies to recover "<<healAmount<<" HP"<<RESET<<endl;
            break;}
        case RE:
            {int resonanceAmount=effect->power+(combo/2);
            changeResonance(resonanceAmount);
            cout<<BLUE<<"Recovered "<<resonanceAmount<<" points of resonance"<<RESET<<endl;
            break;}
        case MI:
            {int weaponBonus = getWeaponBonusDamage();
            int damage=effect->power+harmony+weaponBonus;
            target.takeDamage(damage);
            cout<<RED<<target.getName()<<" got "<<damage<<" points of damage"<<RESET<<endl;
            if(weaponBonus > 0 && weaponUsageCallback){
                weaponUsageCallback();
            }
            break;}
        case FA:
            {int defenseAmount=effect->power+(harmony/2);
            setDefense(getDefense()+defenseAmount);
            cout<<CYAN<<"Aquired "<<defenseAmount<<" points of defense"<<RESET<<endl;
            break;}
        case SOL:
            {if(getDefense()<0)setDefense(0);
            int healAmount=effect->power;
            heal(healAmount);
            cout<<MAGENTA<<"Negative effects Purged. Recovered "<<healAmount<<" points of health"<<RESET<<endl;
            break;}
        case LA:
            {Note randomNote=static_cast<Note>(Utils::randomInt(1,6));
            cout<<WHITE<<"Random notes applied"<<RESET<<endl;
            applyNoteEffect(randomNote,target);
            break;}
        case SI:
            {if(resonance>=30){
                    changeResonance(-30);
                    extraTurns+=2;
                    cout<<WHITE<<"Aquire extra turns"<<RESET<<endl;
                }
                else {
                    cout<<RED<<"Not enough resonance! 30 resonance needed"<<RESET<<endl;
                    // InputSystem::waitForAnyKey();
                }
                
            break;}
        default:
            break;
    }
}
//apply the rune effects
//rune: the rune applied
//target: to the enemy
void CombatPlayer::applyRuneEffect(const Rune &rune,Character &target){
    int effectPower=rune.power+(harmony*2)+(combo/2);
    cout<<BOLD<<rune.color<<"\n"<<rune.name<<" activated"<<RESET<<endl;
    if(rune.name=="Cure Rune"){
        heal(effectPower);
        cout<<GREEN<<effectPower<<" healed"<<RESET<<endl;
    }
    else if(rune.name=="Attack Rune"){
        int totalDamage = effectPower + getWeaponBonusDamage();
        target.takeDamage(totalDamage);
        cout<<RED<<target.getName()<<" took "<<totalDamage<<" points of damage"<<RESET<<endl;
        if(totalDamage > effectPower && weaponUsageCallback){
            weaponUsageCallback();
        }
    }
    else if(rune.name=="Defense Rune"){
        setDefense(getDefense()+effectPower);
        cout<<BLUE<<"Aquired "<<effectPower<<" points of defense"<<RESET<<endl;
    }
    resetCombo();
    // InputSystem::waitForAnyKey();
}
//15 resonance is needed for activating the rune
//runeIndex: the index of applied rune
//target: the target the rune is applied on
void CombatPlayer::activeRune(int runeIndex,Character &target){
    const auto &rune=runes[runeIndex];
    if(resonance>=15){
        changeResonance(-15);
        applyRuneEffect(*rune,target);
        clearMelody();//
        increaseHarmony();
    }
    else {
        cout<<RED<<"Not enough resonance! 15 resonance needed"<<RESET<<endl;
        InputSystem::waitForAnyKey();
    }
}
//reset the defense after each round
void CombatPlayer::resetTurn(){
    defense=0;
}
//return the amount of Weapon Bonus Damage
int CombatPlayer::getWeaponBonusDamage(){
    if(weaponBonusDamage <= 0) return 0;
    if(!weaponNeedsAmmo) return weaponBonusDamage;
    if(consumeAmmoCallback){
        return consumeAmmoCallback() ? weaponBonusDamage : 0;
    }
    return 0;
}
