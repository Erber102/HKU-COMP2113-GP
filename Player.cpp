// Player.cpp
#include "Player.h"
#include "item.h"  
#include <iostream>
#include <algorithm>
#include <string>

using namespace std;

void Player::addItem(Item* item) {
    if (item != nullptr) {
        inventory.push_back(item);
    }
}

bool Player::removeItem(string& itemName, int quantity) {
    if (quantity <= 0) return false;
    
    int removedCount = 0;
    auto it = inventory.begin();
    
    while (it != inventory.end() && removedCount < quantity) {
        if ((*it)->name == itemName) {
            // 删除动态分配的内存
            delete *it;
            it = inventory.erase(it);
            removedCount++;
        } else {
            ++it;
        }
    }
    
    return removedCount > 0;
}

bool Player::hasItem(string& itemName) {
    for (Item* item : inventory) {
        if (item->name == itemName) {
            return true;
        }
    }
    return false;
}

int Player::getItemCount(string& itemName) {
    int count = 0;
    for (Item* item : inventory) {
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
    
    // 统计相同物品的数量
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
    
    // 显示物品
    for (auto& pair : itemCounts) {
        // 这里可以添加获取物品基础价值的方法
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
    if (stamina > 100) stamina = 100;//体力上限
}

void Player::consumeStamina(int amount) {
    stamina -= amount;
    if (stamina < 0) stamina = 0;
}

void Player::clearInventory() {
    // 清理所有动态分配的物品
    for (Item* item : inventory) {
        delete item;
    }
    inventory.clear();
}