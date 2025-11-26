#include "character.h"
#include<iostream>
#include<algorithm>
#include<string>
using namespace std;

string Character::getName()const{return name;}
int Character::getHealth()const{return health;}
int Character::getMaxHealth()const{return maxHealth;}
int Character::getResonance()const{return resonance;}
int Character::getDefense()const{return defense;}

void Character::takeDamage(int damage){
    int realDamage=max(0,damage-defense);
    health=max(0,health-realDamage);
}
void Character::heal(int amount){
    health=min(maxHealth,health+amount);
}
void Character::changeResonance(int amount){
    resonance=max(0,min(100,resonance+amount));
}
void Character::setDefense(int def){
    defense=def;
}
void Character::addRune(unique_ptr<Rune> rune){
    runes.push_back(move(rune));
}
const vector<unique_ptr<Rune>>& Character::getRunes()const{
    return runes;
}
void Character::addNoteEffect(Note note,unique_ptr<NoteEffect> effect){
    noteEffect[note]=move(effect);
}
const NoteEffect* Character::getNoteEffect(Note note)const{
    auto it=noteEffect.find(note);
    return (it!=noteEffect.end())?it->second.get():nullptr;
}
void Character::addNoteToMelody(Note note){
    melody.push_back(note);
}
void Character::clearMelody(){
    melody.clear();
}
const vector<Note> &Character::getMelody()const{
    return melody;
}
bool Character::isAlive()const{
    return health>0;
}
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
string Character::getHealthBar()const{
    int barWidth=min(20,Terminal::getTerminalWidth()/4);
    return Utils::formatHealthBar(health,maxHealth,barWidth);
}
string Character::getResonanceBar()const{
    int barWidth=min(20,Terminal::getTerminalWidth()/4);
    return Utils::formatResonanceBar(resonance,barWidth);
}
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
void Character::applyNoteEffect(Note note,Character &target){
    const NoteEffect *effect=getNoteEffect(note);
    if(effect){
        cout<<BOLD<<effect->color<<" "<<effect->name<<"!"<<RESET<<endl;
        cout<<effect->description<<endl;
    }
}