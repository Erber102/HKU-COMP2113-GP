// Player.h
#ifndef PLAYER_H
#define PLAYER_H

#include "../Core/Common.h"
#include "../Combat/CombatPlayer.h"
#include "../Item/item.h"
#include <vector>
#include <string>
#include <memory>

class Player {
private:
    std::vector<std::unique_ptr<Item>> inventory;
    std::unique_ptr<CombatPlayer> combatPlayer;

public:
    int hp = 100;
    int stamina = 50;
    int money = 0; // Initial value

    // Item management
    void addItem(std::unique_ptr<Item> item);
    bool removeItem(std::string& itemName, int quantity = 1);
    bool hasItem(const std::string& itemName);
    int getItemCount(std::string& itemName);
    void displayInventory();

    // Status management
    bool isAlive();
    void takeDamage(int damage);
    void restoreStamina(int amount);
    void consumeStamina(int amount);

    // Get item list
    const std::vector<std::unique_ptr<Item>>& getInventory() const { return inventory; }

    // Clear inventory (for game reset)
    void clearInventory();

    // Combat system methods - delegated to CombatPlayer
    void initializeCombatPlayer(const std::string& name);
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
