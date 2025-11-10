// Player.h
#ifndef PLAYER_H
#define PLAYER_H

#include "Common.h"
#include <vector>
#include <string>
using namespace std;

class Player {
private:
    vector<Item*> inventory;
    
public:
    int hp = 100;
    int stamina = 50;
    int money = 0; //初始值
    
    
    // 物品管理
    void addItem(Item* item);
    bool removeItem(string& itemName, int quantity = 1);
    bool hasItem(string& itemName);
    int getItemCount(string& itemName);
    void displayInventory();
    
    // 状态管理
    bool isAlive();
    void takeDamage(int damage);
    void restoreStamina(int amount);
    void consumeStamina(int amount);
    
    // 获取物品列表
    vector<Item*>& getInventory() { return inventory; }
    
    // 清空物品栏（用于重置游戏）
    void clearInventory();
};

#endif 
