#include "player.h"
#include "utils.h"
#include<iostream>
#include<algorithm>
using namespace std;

Player::Player(const string &name):Character(name,100),harmony(5),combo(0),extraTurns(0){
    addRune(make_unique<Rune>("Cure Rune",vector<Note>{DO,RE,DO},"Heal",20,GREEN));
    addRune(make_unique<Rune>("Attack Rune",vector<Note>{MI,FA,MI},"Damage",25,RED));
    addRune(make_unique<Rune>("Defense Rune",vector<Note>{SOL,LA,SOL},"Defense",15,BLUE));
    addNoteEffect(DO,make_unique<NoteEffect>("Heal","Recover small amount of health",8,GREEN));
    addNoteEffect(RE,make_unique<NoteEffect>("Power up","Recover resonance",10,BLUE));
    addNoteEffect(MI,make_unique<NoteEffect>("Attack","Damage the enemy",12,RED));
    addNoteEffect(FA,make_unique<NoteEffect>("Shield","aquire temper defense",5,CYAN));
    addNoteEffect(SOL,make_unique<NoteEffect>("Purge","Rurge and recover small amount of health",5,MAGENTA));
    addNoteEffect(LA,make_unique<NoteEffect>("Variation","Random effect",0,WHITE));
    addNoteEffect(SI,make_unique<NoteEffect>("Accumulation","Extra turns",0,WHITE));
}

void Player::increaseHarmony(){
    harmony=min(10,harmony+1);
}
void Player::increaseCombo(){
    combo++;
}
void Player::resetCombo(){
    combo=0;
}
int Player::getCombo(){
    return combo;
}
int Player::getHarmony(){
    return harmony;
}
bool Player::hasExtraTurns(){
    return extraTurns>0;
}
void Player::useExtraTurn(){
    if(extraTurns>0)extraTurns--;
}
//check if the rune is activated
bool Player::activateRune(Character &target){
    int activeRuneIndex=checkMelody();
    if(activeRuneIndex==-1)return 0;
    const auto &activeRune=runes[activeRuneIndex];
    cout<<BOLD<<activeRune->color<<" !Detecting active rune: "<<activeRune->name<<"! "<<RESET<<endl;
    cout<<"Effect: "<<activeRune->effect<<endl;
    cout<<YELLOW<<"Press "<<RED<<"[Space]"<<YELLOW<<" to activate rune, or press other keys to continue..."<<RESET<<endl;
    while(1){
        if(Terminal::kbhit()){
            char key=Terminal::getch();
            if(key==' '){
                Player::activeRune(activeRuneIndex,target);
                return 1;
            }
            else {
                break;
            }
        }
        Utils::sleepMs(100);
    }
    return 0;
}
void Player::takeTurn(Character &target){
    Terminal::clearScreen();
    Terminal::drawTitle("Your Turn");
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
        if(Terminal::kbhit()){
            char key=Terminal::getch();
            handleInput(key,target);
            break;
        }
        Utils::sleepMs(100);
    }
    changeResonance(5);
}
void Player::showNoteInputMenu(){
    Terminal::drawSeparator();
    cout<<BOLD<<"Play note: "<<RESET<<endl;
    //for instructions
    cout<<" [1] DO  - "<<GREEN<<"Heal"<<RESET<<endl;
    cout<<" [2] RE  - "<<BLUE<<"Power up"<<RESET<<endl;
    cout<<" [3] MI  - "<<RED<<"Attack"<<RESET<<endl;
    cout<<" [4] FA  - "<<CYAN<<"Shield"<<RESET<<endl;
    cout<<" [5] SOL - "<<YELLOW<<"Purge"<<RESET<<endl;
    cout<<" [6] LA  - "<<MAGENTA<<"Variation"<<RESET<<endl;
    cout<<" [7] SI  - "<<WHITE<<"Extra turns"<<RESET<<endl;
    cout<<endl;
}
void Player::handleInput(char key,Character &target){
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
    if(key!=' ')Utils::waitForAnyKey();
}
//applying the specific note effect
void Player::applyNoteEffect(Note note,Character &target){
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
                    // Utils::waitForAnyKey();
                }
                
            break;}
        default:
            break;
    }
}
void Player::applyRuneEffect(const Rune &rune,Character &target){
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
    // Utils::waitForAnyKey();
}
// 15 resonance is needed for activating the rune
void Player::activeRune(int runeIndex,Character &target){
    const auto &rune=runes[runeIndex];
    if(resonance>=15){
        changeResonance(-15);
        applyRuneEffect(*rune,target);
        clearMelody();//
        increaseHarmony();
    }
    else {
        cout<<RED<<"Not enough resonance! 15 resonance needed"<<RESET<<endl;
        Utils::waitForAnyKey();
    }
}
void Player::resetTurn(){
    defense=0;
}