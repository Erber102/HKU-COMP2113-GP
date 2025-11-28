#include "character.h"
#include<iostream>
#include<algorithm>
#include<string>
using namespace std;
//return the value of corresponding variables
string Character::getName()const{return name;}
int Character::getHealth()const{return health;}
int Character::getMaxHealth()const{return maxHealth;}
int Character::getResonance()const{return resonance;}
int Character::getDefense()const{return defense;}

//apply the damge taken
//damage: the amount of damage taken
void Character::takeDamage(int damage){
    int realDamage=max(0,damage-defense);
    health=max(0,health-realDamage);
}
//recover the health
//amount:the amount of health been healed
void Character::heal(int amount){
    health=min(maxHealth,health+amount);
}
//change resonance
//amount: the amount of changed made in resonance
void Character::changeResonance(int amount){
    resonance=max(0,min(100,resonance+amount));
}
//set the amount of defense to "def"
//def: amount of defense
void Character::setDefense(int def){
    defense=def;
}
//adding the rune for saving
//rune: the rune applied
void Character::addRune(unique_ptr<Rune> rune){
    runes.push_back(move(rune));
}
//return the vector containing the runes
const vector<unique_ptr<Rune>>& Character::getRunes()const{
    return runes;
}
//match "note" with its corresponding "effect"
//note: certain note
//effect: word description of effect
void Character::addNoteEffect(Note note,unique_ptr<NoteEffect> effect){
    noteEffect[note]=move(effect);
}
//find and return the effect of the inputed "note"
//note: note needed to find
const NoteEffect* Character::getNoteEffect(Note note)const{
    auto it=noteEffect.find(note);
    return (it!=noteEffect.end())?it->second.get():nullptr;
}
//add the "note" to melody for display
//note: the node added
void Character::addNoteToMelody(Note note){
    melody.push_back(note);
}
//clear the queue of melody
void Character::clearMelody(){
    melody.clear();
}
//return the vector containing the melodies
const vector<Note> &Character::getMelody()const{
    return melody;
}
//check if the character is alive
//returns 1 for alive and else 0
bool Character::isAlive()const{
    return health>0;
}
//check if any melody matches the rune
//returns the index of the available rune
int Character::checkMelody(){
    for(int i=0;i<(int)runes.size();i++){
        const auto &rune=runes[i];
        if(melody.size()<rune->sequence.size())continue;
        bool match=1;
        for(int j=0;j<(int)rune->sequence.size();j++){
            if(melody[melody.size()-rune->sequence.size()+j]!=rune->sequence[j]){
                match=0;
                break;
            }
        }
        if(match)return i;
    }
    return -1;
}
//display the health bar
//returns the displayed bar
string Character::getHealthBar()const{
    int barWidth=min(20,Terminal::getTerminalWidth()/4);
    return Utils::formatHealthBar(health,maxHealth,barWidth);
}
//display the resonance bar
//returns the displayed bar
string Character::getResonanceBar()const{
    int barWidth=min(20,Terminal::getTerminalWidth()/4);
    return Utils::formatResonanceBar(resonance,barWidth);
}
//display the melody queue
//returns the displayed queue
string Character::getMelodyDisplay()const{
    string display;
    if(melody.empty())display=YELLOW+string("Current melody: Empty")+RESET;
    else {
        display=CYAN+string("Current melody: ");
        for(Note n:melody){
            display+=Utils::noteToString(n)+" ";
        }
        display+=RESET;
    }
    return display;
}
//display the effects of the "note"
//note: the note applied
//target: the target the note is applied on
void Character::applyNoteEffect(Note note,Character &target){
    const NoteEffect *effect=getNoteEffect(note);
    if(effect){
        cout<<BOLD<<effect->color<<" "<<effect->name<<"!"<<RESET<<endl;
        cout<<effect->description<<endl;
    }
}
