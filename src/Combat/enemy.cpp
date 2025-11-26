#include "enemy.h"
#include "CombatPlayer.h"
#include "../Core/InputSystem.h"
#include "utils.h"
#include<iostream>
#include<memory>
#include<cstdlib>
#include<ctime>
using namespace std;

Enemy::Enemy(const string &name,const string &enemyType,int health):Character(name,health),type(enemyType),turnCounter(0){
    if(type=="Rhythm Breaker"){
        description={"Piercing noise emitting!","Your rhythm is disrupted!","Playing dissonant notes!"};
        addRune(std::unique_ptr<Rune>(new Rune("Noise rune",vector<Note>{SI,RE,SI},"Cause damage and reduce resonance",15,RED)));
    }
    else if(type=="Melody Thief"){
        description={"Your melody stolen!","Imitating your playing!","Disturbing the music!"};
        addRune(std::unique_ptr<Rune>(new Rune("Theft rune",vector<Note>{FA,SOL,FA},"Theft resonance",10,MAGENTA)));
    }
    else if (type=="Harmony Monster"){
        description={"Producing defening harmony!","Low frequencies vibrating!","Creating discordant chords"};
        addRune(std::unique_ptr<Rune>(new Rune("Shock rune",vector<Note>{DO,SI,DO},"Cause a great amount of damage",30,RED)));
    }
}
// Enemy's action in one turn
void Enemy::takeTurn(Character &target){
    InputSystem::clearScreen();
    InputSystem::drawTitle(name+"'s turn");
    cout<<BOLD<<"Enemy: "<<name<<RESET<<endl;
    cout<<"Health: "<<getHealthBar()<<endl<<endl;
    //Enemy's behavior
    if(turnCounter%3==0&&resonance>=10)useRuneAttack(target);
    else if(turnCounter%2==0)disruptPlayer(target);
    else normalAttack(target);
    turnCounter++;
    changeResonance(5);
    InputSystem::waitForAnyKey();
}
void Enemy::useRuneAttack(Character &target){
    if(runes.empty())return ;
    const auto &rune=runes[0];
    changeResonance(-10);
    cout<<RED<<getRandomDescription()<<RESET<<endl;
    cout<<BOLD<<rune->color<<name<<" used "<<rune->name<<"!"<<RESET<<endl;
    if(rune->name=="Noise rune"){
        target.takeDamage(rune->power);
        static_cast<CombatPlayer&>(target).changeResonance(-10);
        cout<<RED<<"You took "<<rune->power<<" points of damage, and resonance is reduced!"<<RESET<<endl;
    }
    else if(rune->name=="Theft rune"){
        int stolen=min(15,static_cast<CombatPlayer&>(target).getResonance());
        static_cast<CombatPlayer&>(target).changeResonance(-stolen);
        changeResonance(stolen);
        cout<<MAGENTA<<name<<" stole "<<stolen<<" points of resonance!"<<RESET<<endl;
    }
    else if(rune->name=="Shock rune"){
        target.takeDamage(rune->power);
        cout<<RED<<"You took "<<rune->power<<" points of damage!"<<RESET<<endl;
    }
}
void Enemy::disruptPlayer(Character &target){
    cout<<YELLOW<<getRandomDescription()<<RESET<<endl;
    if(rand()%100<50){
        static_cast<CombatPlayer&>(target).clearMelody();
        cout<<RED<<"Your melody was cleared!"<<RESET<<endl;
    }
    if(rand()%100<30){
        static_cast<CombatPlayer&>(target).changeResonance(-8);
        cout<<RED<<"Your resonance reduced!"<<RESET<<endl;
    }
    if(rand()%100<20){
        static_cast<CombatPlayer&>(target).resetCombo();
        cout<<RED<<"Your combo has been reset!"<<RESET<<endl;
    }
}
void Enemy::normalAttack(Character &target){
    int damage=10+(rand()%6);
    target.takeDamage(damage);
    cout<<RED<<name<<" caused you "<<damage<<" points of damage"<<RESET<<endl;
}
string Enemy::getRandomDescription(){
    return description[rand()%description.size()];
}
void Enemy::resetTurn(){
    defense=0;
}