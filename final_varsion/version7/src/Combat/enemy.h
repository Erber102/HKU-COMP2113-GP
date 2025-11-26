#ifndef ENEMY_H
#define ENEMY_H

#include "character.h"
#include<string>
#include<vector>
using namespace std;
class Enemy:public Character{
private:
    vector<string> description;
    string type;
    int turnCounter;
    void useRuneAttack(Character &target);
    void disruptPlayer(Character &target);
    void normalAttack(Character &target);
    string getRandomDescription();
public:
    Enemy(const string &name,const string &enemyType,int health);
    void takeTurn(Character &target);
    void resetTurn();
};

#endif