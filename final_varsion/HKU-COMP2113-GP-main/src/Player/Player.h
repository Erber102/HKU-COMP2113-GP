// Player.h
#ifndef PLAYER_H
#define PLAYER_H

#include "../Core/Common.h"
#include "../Combat/CombatPlayer.h"
#include <vector>
#include <string>
#include <memory>
using namespace std;

class Player {
private:
    vector<Item*> inventory;
    unique_ptr<CombatPlayer> combatPlayer;

public:
    int hp = 100;
    int stamina = 50;
    int money = 0; // Initial value

    // Item management
    void addItem(Item* item);
    bool removeItem(string& itemName, int quantity = 1);
    bool hasItem(const string& itemName);
    int getItemCount(string& itemName);
    Item* getBestWeapon();
    bool removeSpecificItem(Item* target);
    void displayInventory();

    // Status management
    bool isAlive();
    void takeDamage(int damage);
    void restoreStamina(int amount);
    void consumeStamina(int amount);

    // Get item list
    vector<Item*>& getInventory() { return inventory; }

    // Clear inventory (for game reset)
    void clearInventory();

    // Combat system methods - delegated to CombatPlayer
    void initializeCombatPlayer(const string& name);
    CombatPlayer* getCombatPlayer() { return combatPlayer.get(); }
    void increaseHarmony();
    void increaseCombo();
    void resetCombo();
    int getCombo();
    int getHarmony();
    bool hasExtraTurns();
    void useExtraTurn();
    void resetTurn();

    // Compatibility methods - for DayPhase
    int getStamina() { return stamina; }
    int getMaxStamina() { return 100; } // Assume max stamina is 100
    void setStamina(int newStamina) { stamina = newStamina; }
};

#endif 
