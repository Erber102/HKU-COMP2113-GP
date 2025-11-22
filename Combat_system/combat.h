#ifndef COMBAT_H
#define COMBAT_H

#include "player.h"
#include "enemy.h"
#include<vector>
#include<memory>

//The class for combat actions;
class Combat{
private:
    unique_ptr<Player> player;
    vector<unique_ptr<Enemy>> enemies;
    int currentEnemyIndex;
    int score;
    void showRuneIntro();
    void showVictory();
    void showGameOver();
public:
    Combat(const string &ply);
    ~Combat();
    void fightEnemy(int enemyIndex);
};

#endif