#include "CombatPlayer.h"
#include "../Core/InputSystem.h"
#include "utils.h"
#include<iostream>
#include<memory>
#include "backpack.h"
#include<algorithm>
using namespace std;

CombatPlayer::CombatPlayer(const string &name):Character(name,100),harmony(5),combo(0),extraTurns(0){
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
    initBackpack();
}
void CombatPlayer::initBackpack() {
    initBackpack(backpack, "战斗背包", 10);
}

void CombatPlayer::increaseHarmony(){
    harmony=min(10,harmony+1);
}
void CombatPlayer::increaseCombo(){
    combo++;
}
void CombatPlayer::resetCombo(){
    combo=0;
}
int CombatPlayer::getCombo(){
    return combo;
}
int CombatPlayer::getHarmony(){
    return harmony;
}
bool CombatPlayer::hasExtraTurns(){
    return extraTurns>0;
}
void CombatPlayer::useExtraTurn(){
    if(extraTurns>0)extraTurns--;
}
//check if the rune is activated
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
                break;
            }
        }
        InputSystem::sleepMs(100);
    }
    return 0;
}
void CombatPlayer::takeTurn(Character &target){
    InputSystem::clearScreen();
    InputSystem::drawTitle("Your Turn");
    
    // 检查生命值，如果过低提示使用道具
    if (getHealth() < 30) {
        cout << RED << "警告：生命值过低！" << RESET << endl;
        
        // 检查背包中是否有恢复道具
        bool hasRecoveryItem = false;
        for (int i = 0; i < backpack.itemCount; i++) {
            if (backpack.items[i]->isEdible && backpack.items[i]->healthEffect > 0) {
                hasRecoveryItem = true;
                break;
            }
        }
        
        if (hasRecoveryItem) {
            cout << YELLOW << "背包中有恢复道具，按 [8] 查看并使用" << RESET << endl;
        }
        cout << endl;
    }
    
    //display player health
    cout<<endl<<"Health:    "<<getHealthBar();
    cout<<endl;

    
            // cout<<"!!"<<getResonance()<<endl;
    //display player health
    cout<<endl<<"Health:    "<<getHealthBar();
    cout<<endl;
    //display resonance
    cout<<"Resonance: "<<getResonanceBar();
    cout<<" Harmony: "<<YELLOW<<harmony<<"/10"<<RESET;
    cout<<" | Combo: "<<CYAN<<combo<<RESET;
    if(extraTurns>0){
        cout<<" | extra turn: "<<GREEN<< extraTurns<<RESET;
    }
    cout<<endl;
    //display current melody
    cout<<getMelodyDisplay()<<endl<<endl;
    showNoteInputMenu();
    if(activateRune(target))return ;
    cout<<YELLOW<<"Please select an operation to continue..."<<RESET<<endl;
    while(1){
        if(InputSystem::kbhit()){
            char key=InputSystem::getch();
            handleInput(key,target);
            break;
        }
        InputSystem::sleepMs(100);
    }
    changeResonance(5);
}
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
    cout<<" [8] B   - "<<WHITE<<"Check backpack"<<RESET<<endl;
    cout<<endl;
}
void CombatPlayer::handleInput(char key,Character &target){
    bool notePlayed=1;
    //relate the keys to the effects
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
        case '8':  // 新增背包查看
        case 'b':
        case 'B':
            showBackpackAndUseItem(target);
            notePlayed = 0;
            break;
        default:
            cout<<RED<<"Invalid input! Please use 1-8 to play note"<<RESET<<endl;
            notePlayed=0;
    }
    if(notePlayed){
        //check and increase combo
        increaseCombo();
        if(combo%5==0){
            cout<<BOLD<<CYAN<<"Combo "<<combo<<"! Resonance increased !"<<RESET<<endl;
            changeResonance(combo);
        }
        //check if there are activated rune
        activateRune(target);
    }
    if(key!=' ')InputSystem::waitForAnyKey();
}
//applying the specific note effect
void CombatPlayer::applyNoteEffect(Note note,Character &target){
    const NoteEffect *effect=getNoteEffect(note);
    if(!effect)return ;
    cout<<BOLD<<effect->color<<effect->name<<"!"<<RESET<<endl;
    switch(note){
        case DO:
            {int healAmount=effect->power+harmony;
            heal(healAmount);
            cout<<GREEN<<"Recovered "<<healAmount<<" points of health"<<RESET<<endl;
            break;}
        case RE:
            {int resonanceAmount=effect->power+(combo/2);
            changeResonance(resonanceAmount);
            cout<<BLUE<<"Recovered "<<resonanceAmount<<" points of resonance"<<RESET<<endl;
            break;}
        case MI:
            {int damage=effect->power+harmony;
            target.takeDamage(damage);
            cout<<RED<<target.getName()<<" got "<<damage<<" points of damage"<<RESET<<endl;
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
            {if(resonance>=10){
                    changeResonance(-10);
                    extraTurns+=2;
                    cout<<WHITE<<"Aquire extra turns"<<RESET<<endl;
                }
                else {
                    cout<<RED<<"Not enough resonance! 5 resonance needed"<<RESET<<endl;
                    // InputSystem::waitForAnyKey();
                }
                
            break;}
        default:
            break;
    }
}
void CombatPlayer::applyRuneEffect(const Rune &rune,Character &target){
    int effectPower=rune.power+(harmony*2)+(combo/2);
    cout<<BOLD<<rune.color<<"\n"<<rune.name<<" activated"<<RESET<<endl;
    if(rune.name=="Cure Rune"){
        heal(effectPower);
        cout<<GREEN<<effectPower<<" healed"<<RESET<<endl;
    }
    else if(rune.name=="Attack Rune"){
        target.takeDamage(effectPower);
        cout<<RED<<target.getName()<<" took "<<effectPower<<" points of damage"<<RESET<<endl;
    }
    else if(rune.name=="Defense Rune"){
        setDefense(getDefense()+effectPower);
        cout<<BLUE<<"Aquired "<<effectPower<<" points of defense"<<RESET<<endl;
    }
    resetCombo();
    // InputSystem::waitForAnyKey();
}
// 15 resonance is needed for activating the rune
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
void CombatPlayer::resetTurn(){
    defense=0;
}

void CombatPlayer::showBackpackAndUseItem(Character &target) {
    InputSystem::clearScreen();
    InputSystem::drawTitle("背包物品");
    
    if (backpack.itemCount == 0) {
        cout << RED << "背包为空！" << RESET << endl;
        InputSystem::waitForAnyKey();
        return;
    }
    
    // 显示背包内容
    displayBackpackItems(backpack);
    
    // 检查是否有可食用道具
    bool hasEdible = false;
    for (int i = 0; i < backpack.itemCount; i++) {
        if (backpack.items[i]->isEdible) {
            hasEdible = true;
            break;
        }
    }
    
    if (!hasEdible) {
        cout << YELLOW << "背包中没有可食用道具" << RESET << endl;
        InputSystem::waitForAnyKey();
        return;
    }
    
    // 让玩家选择使用道具
    cout << YELLOW << "输入道具编号使用（输入0取消）: " << RESET;
    
    while (true) {
        if (InputSystem::kbhit()) {
            char choice = InputSystem::getch();
            
            if (choice == '0') {
                cout << "取消使用道具" << endl;
                break;
            }
            
            int itemIndex = choice - '1';  // 转换为索引（1-based转0-based）
            if (itemIndex >= 0 && itemIndex < backpack.itemCount) {
                Item* selectedItem = getItemFromBackpack(backpack, itemIndex);
                if (selectedItem && selectedItem->isEdible) {
                    // 使用道具
                    useItemFromBackpack(backpack, itemIndex);
                    cout << GREEN << "使用了 " << selectedItem->name << RESET << endl;
                    
                    // 应用治疗效果
                    int healAmount = selectedItem->healthEffect;
                    heal(healAmount);
                    cout << GREEN << "恢复了 " << healAmount << " 点生命值" << RESET << endl;
                    
                    // 如果还有体力效果
                    if (selectedItem->staminaEffect > 0) {
                        // 恢复共鸣值
                        changeResonance(selectedItem->staminaEffect);
                        cout << BLUE << "恢复了 " << selectedItem->staminaEffect << " 点共鸣" << RESET << endl;
                    }
                    
                    // 使用道具后结束当前回合
                    cout << YELLOW << "使用道具结束当前回合" << RESET << endl;
                    InputSystem::waitForAnyKey();
                    return;
                } else {
                    cout << RED << "该道具无法使用！" << RESET << endl;
                }
            } else {
                cout << RED << "无效的选择！" << RESET << endl;
            }
        }
        InputSystem::sleepMs(100);
    }
    
    InputSystem::waitForAnyKey();
}
