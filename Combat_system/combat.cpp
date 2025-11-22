#include "combat.h"
#include "utils.h"
#include<iostream>
#include<cstdlib>
#include<ctime>
using namespace std;
//Initializing the player and enemies
Combat::Combat(const string &ply):currentEnemyIndex(0),score(0){
    srand(time(0));
    player=make_unique<Player>(ply);
    enemies.push_back(make_unique<Enemy>("Noise Monster","Rhythm Breaker",60));
    enemies.push_back(make_unique<Enemy>("Melody Thief","Melody Thief",70));
    enemies.push_back(make_unique<Enemy>("Harmony Monster","Harmony Monster",100));
    enemies.push_back(make_unique<Enemy>("Final Symphony","Rhythm Breaker",120));
    Terminal::setNonBlockingInput();
}
//restoring the setting of the terminal
Combat::~Combat(){
    Terminal::restoreTerminal();
}
//use the index to distinguish the enemy the player is dealing with
void Combat::fightEnemy(int enemyIndex){
    Enemy &enemy=*enemies[enemyIndex];
    //show the name of encountered enemy
    Terminal::clearScreen();
    cout<<BOLD<<"\nYour encountered "<<RED<<enemy.getName()<<"!"<<RESET<<endl;
    showRuneIntro();
    Utils::sleepMs(3000);
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
    Terminal::clearScreen();
    if(player->isAlive())showVictory();
    else showGameOver();
}
//intro panel to the rune
void Combat::showRuneIntro(){
    cout<<BOLD<<MAGENTA;
    Terminal::drawSeparator();
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
    Terminal::clearScreen();
    cout<<BOLD<<GREEN;
    Terminal::drawTitle("Congratulations! Your have defeated the enemy!");
    Utils::sleepMs(2000);
    cout<<RESET;
}
void Combat::showGameOver(){
    Terminal::clearScreen();
    cout<<BOLD<<RED;
    Terminal::drawTitle("You failed to defeat the enemy.");
    Utils::sleepMs(2000);
    cout<<RESET;
}