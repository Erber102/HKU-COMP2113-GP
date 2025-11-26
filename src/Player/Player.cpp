// Player.cpp
#include "Player.h"
#include "../Item/item.h"  
#include <iostream>
#include <algorithm>
#include <string>

using namespace std;

void Player::addItem(std::unique_ptr<Item> item) {
    if (item != nullptr) {
        inventory.push_back(std::move(item));
    }
}

bool Player::removeItem(string& itemName, int quantity) {
    if (quantity <= 0) return false;

    int removedCount = 0;
    auto it = inventory.begin();

    while (it != inventory.end() && removedCount < quantity) {
        if ((*it)->name == itemName) {
            // No explicit delete needed - unique_ptr handles it automatically
            it = inventory.erase(it);
            removedCount++;
        } else {
            ++it;
        }
    }

    return removedCount > 0;
}

bool Player::hasItem(const string& itemName) {
    for (const auto& item : inventory) {
        if (item->name == itemName) {
            return true;
        }
    }
    return false;
}

int Player::getItemCount(string& itemName) {
    int count = 0;
    for (const auto& item : inventory) {
        if (item->name == itemName) {
            count++;
        }
    }
    return count;
}

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
    for (const auto& item : inventory) {
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

bool Player::isAlive() {
    return hp > 0;
}

void Player::takeDamage(int damage) {
    hp -= damage;
    if (hp < 0) hp = 0;
}

void Player::restoreStamina(int amount) {
    stamina += amount;
    if (stamina > 100) stamina = 100; // Stamina cap
}

void Player::consumeStamina(int amount) {
    stamina -= amount;
    if (stamina < 0) stamina = 0;
}

void Player::clearInventory() {
    // No explicit delete needed - unique_ptr handles it automatically
    inventory.clear();
}

// Combat system method implementations - delegated to CombatPlayer
void Player::initializeCombatPlayer(const string& name) {
    combatPlayer = std::unique_ptr<CombatPlayer>(new CombatPlayer(name));
}

void Player::increaseHarmony() {
    if (combatPlayer) combatPlayer->increaseHarmony();
}

void Player::increaseCombo() {
    if (combatPlayer) combatPlayer->increaseCombo();
}

void Player::resetCombo() {
    if (combatPlayer) combatPlayer->resetCombo();
}

int Player::getCombo() {
    return combatPlayer ? combatPlayer->getCombo() : 0;
}

int Player::getHarmony() {
    return combatPlayer ? combatPlayer->getHarmony() : 0;
}

bool Player::hasExtraTurns() {
    return combatPlayer ? combatPlayer->hasExtraTurns() : false;
}

void Player::useExtraTurn() {
    if (combatPlayer) combatPlayer->useExtraTurn();
}

void Player::resetTurn() {
    if (combatPlayer) combatPlayer->resetTurn();
}