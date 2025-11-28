// Player.cpp
#include "Player.h"
#include "../Item/item.h"  
#include <iostream>
#include <algorithm>
#include <string>

using namespace std;

// Adds an item to player's inventory
// What it does: Adds item pointer to inventory vector if not null
// Inputs: item - Pointer to Item object to add
// Outputs: None
void Player::addItem(Item* item) {
    if (item != nullptr) {
        inventory.push_back(item);
    }
}

// Removes specified quantity of items from inventory
// What it does: Searches for items by name and removes specified quantity
// Inputs: itemName - Name of item to remove
//         quantity - Number of items to remove
// Outputs: bool - True if items were removed, false otherwise
bool Player::removeItem(string& itemName, int quantity) {
    if (quantity <= 0) return false;
    
    int removedCount = 0;
    auto it = inventory.begin();
    
    while (it != inventory.end() && removedCount < quantity) {
        if ((*it)->name == itemName) {
            // Delete dynamically allocated memory
            delete *it;
            it = inventory.erase(it);
            removedCount++;
        } else {
            ++it;
        }
    }
    
    return removedCount > 0;
}

// Checks if player has a specific item
// What it does: Searches inventory for item with matching name
// Inputs: itemName - Name of item to check for
// Outputs: bool - True if item exists in inventory, false otherwise
bool Player::hasItem(const string& itemName) {
    for (Item* item : inventory) {
        if (item->name == itemName) {
            return true;
        }
    }
    return false;
}

// Gets the count of a specific item in inventory
// What it does: Counts how many items with given name exist in inventory
// Inputs: itemName - Name of item to count
// Outputs: int - Number of items with matching name
int Player::getItemCount(string& itemName) {
    int count = 0;
    for (Item* item : inventory) {
        if (item->name == itemName) {
            count++;
        }
    }
    return count;
}

// Gets the best weapon from inventory
// What it does: Finds weapon with highest damage value in inventory
// Inputs: None
// Outputs: Item* - Pointer to best weapon, or nullptr if no weapons
Item* Player::getBestWeapon() {
    Item* bestWeapon = nullptr;
    for (Item* item : inventory) {
        if (item && item->category == CATEGORY_WEAPON) {
            if (!bestWeapon || item->damage > bestWeapon->damage) {
                bestWeapon = item;
            }
        }
    }
    return bestWeapon;
}

// Removes a specific item instance from inventory
// What it does: Finds and removes exact item pointer from inventory
// Inputs: target - Pointer to specific item to remove
// Outputs: bool - True if item was found and removed, false otherwise
bool Player::removeSpecificItem(Item* target) {
    if (!target) return false;
    for (auto it = inventory.begin(); it != inventory.end(); ++it) {
        if (*it == target) {
            delete *it;
            inventory.erase(it);
            return true;
        }
    }
    return false;
}

// Displays player's inventory and status
// What it does: Shows player stats and grouped inventory items with quantities
// Inputs: None
// Outputs: None
void Player::displayInventory() {
    cout << "=== BASICS ===" << endl;
    cout << "HP: " << hp << " | STAMINA: " << stamina << " | MONEY: " << money << endl;
    cout << "BACKPACK:" << endl;

    if (inventory.empty()) {
        cout << "EMPTY" << endl;
        return;
    }

    // Count quantities of same items
    vector<pair<string, int>> itemCounts;
    for (Item* item : inventory) {
        bool found = false;
        for (auto& pair : itemCounts) {
            if (pair.first == item->name) {
                pair.second++;
                found = true;
                break;
            }
        }
        if (!found) {
            itemCounts.push_back({item->name, 1});
        }
    }

    // Display items
    for (auto& pair : itemCounts) {
        // Here you can add method to get item base value
        cout << "  " << pair.first << " x" << pair.second << endl;
    }
}

// Checks if player is alive
// What it does: Returns true if player's health is greater than 0
// Inputs: None
// Outputs: bool - True if player is alive, false otherwise
bool Player::isAlive() {
    return hp > 0;
}

// Applies damage to player
// What it does: Reduces player's health by damage amount, minimum 0
// Inputs: damage - Amount of damage to apply
// Outputs: None
void Player::takeDamage(int damage) {
    hp -= damage;
    if (hp < 0) hp = 0;
}

// Restores player's stamina
// What it does: Increases stamina by amount, capped at maximum 100
// Inputs: amount - Amount of stamina to restore
// Outputs: None
void Player::restoreStamina(int amount) {
    stamina += amount;
    if (stamina > 100) stamina = 100; // Stamina cap
}

// Consumes player's stamina
// What it does: Decreases stamina by amount, minimum 0
// Inputs: amount - Amount of stamina to consume
// Outputs: None
void Player::consumeStamina(int amount) {
    stamina -= amount;
    if (stamina < 0) stamina = 0;
}

// Clears all items from inventory
// What it does: Deletes all dynamically allocated items and clears inventory vector
// Inputs: None
// Outputs: None
void Player::clearInventory() {
    // Clean up all dynamically allocated items
    for (Item* item : inventory) {
        delete item;
    }
    inventory.clear();
}

// Initializes combat player component
// What it does: Creates CombatPlayer instance for combat system
// Inputs: name - Name for the combat player
// Outputs: None
void Player::initializeCombatPlayer(const string& name) {
    combatPlayer = std::unique_ptr<CombatPlayer>(new CombatPlayer(name));
}

// Increases harmony stat for combat player
// What it does: Delegates to CombatPlayer's increaseHarmony method
// Inputs: None
// Outputs: None
void Player::increaseHarmony() {
    if (combatPlayer) combatPlayer->increaseHarmony();
}

// Increases combo counter for combat player
// What it does: Delegates to CombatPlayer's increaseCombo method
// Inputs: None
// Outputs: None
void Player::increaseCombo() {
    if (combatPlayer) combatPlayer->increaseCombo();
}

// Resets combo counter for combat player
// What it does: Delegates to CombatPlayer's resetCombo method
// Inputs: None
// Outputs: None
void Player::resetCombo() {
    if (combatPlayer) combatPlayer->resetCombo();
}

// Gets current combo count
// What it does: Returns CombatPlayer's combo count, or 0 if no combat player
// Inputs: None
// Outputs: int - Current combo count
int Player::getCombo() {
    return combatPlayer ? combatPlayer->getCombo() : 0;
}

// Gets current harmony level
// What it does: Returns CombatPlayer's harmony level, or 0 if no combat player
// Inputs: None
// Outputs: int - Current harmony level
int Player::getHarmony() {
    return combatPlayer ? combatPlayer->getHarmony() : 0;
}

// Checks if player has extra turns available
// What it does: Returns CombatPlayer's extra turns status, or false if no combat player
// Inputs: None
// Outputs: bool - True if extra turns available, false otherwise
bool Player::hasExtraTurns() {
    return combatPlayer ? combatPlayer->hasExtraTurns() : false;
}

// Uses one extra turn
// What it does: Delegates to CombatPlayer's useExtraTurn method
// Inputs: None
// Outputs: None
void Player::useExtraTurn() {
    if (combatPlayer) combatPlayer->useExtraTurn();
}

// Resets turn state for combat player
// What it does: Delegates to CombatPlayer's resetTurn method
// Inputs: None
// Outputs: None
void Player::resetTurn() {
    if (combatPlayer) combatPlayer->resetTurn();
}