#ifndef COMBAT_H
#define COMBAT_H

#include "CombatPlayer.h"
#include "enemy.h"
#include<vector>
#include<memory>
#include<string>

class Player;
struct Item;

//The class for combat actions;
class Combat{
private:
    struct EquippedWeapon {
        Item* itemPtr;
        std::string name;
        int damage;
        bool needsAmmo;
        std::string ammoItem;
    };

    unique_ptr<CombatPlayer> player;
    Player* playerRef;
    vector<unique_ptr<Enemy>> enemies;
    int currentEnemyIndex;
    int score;
    EquippedWeapon currentWeapon;

    void showRuneIntro();
    void showVictory();
    void showGameOver();
    int useHealingItem();
    bool consumeAmmoForCurrentWeapon();
    void openWeaponMenu();
    void applyEquippedWeapon();
    void setCurrentWeapon(Item* weaponPtr);
    void handleWeaponDurability();
    int calculateReward(int enemyIndex) const;
public:
    Combat(Player* owner, const string &ply, Item* startingWeapon);
    ~Combat();
    void fightEnemy(int enemyIndex);
    void equipWeapon(Item* weaponPtr);
};

#endif