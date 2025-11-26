#include "backpack.h"
#include <iostream>

using namespace std;

// 初始化背包
void initBackpack(Backpack &backpack, string name, int capacity) {
    backpack.name = name;
    backpack.capacity = capacity;
    backpack.itemCount = 0;
}

// 添加物品到背包
bool addItemToBackpack(Backpack &backpack, Item* item) {
    if (backpack.itemCount < backpack.capacity) {
        backpack.items[backpack.itemCount] = item;
        backpack.itemCount++;
        return true;
    }
    return false;
}

// 从背包移除物品
void removeItemFromBackpack(Backpack &backpack, int index) {
    if (index >= 0 && index < backpack.itemCount) {
        // 删除动态分配的内存
        delete backpack.items[index];
        
        // 移动后续指针
        for (int i = index; i < backpack.itemCount - 1; i++) {
            backpack.items[i] = backpack.items[i + 1];
        }
        backpack.itemCount--;
    }
}

// 显示背包内容
void showBackpackContents(Backpack &backpack) {
    cout << "=== " << backpack.name << " (" << backpack.itemCount << "/" << backpack.capacity << ") ===" << endl;
    if (backpack.itemCount == 0) {
        cout << "背包为空" << endl;
        return;
    }
    
    for (int i = 0; i < backpack.itemCount; i++) {
        cout << i+1 << ". " << backpack.items[i]->name << " (价值:" << backpack.items[i]->value << ")";
        if (backpack.items[i]->isEdible) {
            cout << " [可食用]";
        }
        cout << endl;
    }
}

// 计算背包总价值
int calculateBackpackTotalValue(Backpack &backpack) {
    int total = 0;
    for (int i = 0; i < backpack.itemCount; i++) {
        total += backpack.items[i]->value;
    }
    return total;
}

// 使用背包中的物品
void useItemFromBackpack(Backpack &backpack, int itemIndex) {
    if (itemIndex < 0 || itemIndex >= backpack.itemCount) {
        cout << "无效的道具索引!" << endl;
        return;
    }
    
    Item* item = backpack.items[itemIndex];
    
    if (item->isEdible) {
        cout << "你食用了 " << item->name << endl;
        cout << "生命值恢复了 " << item->healthEffect << " 点" << endl;
        cout << "体力值恢复了 " << item->staminaEffect << " 点" << endl;
        
        // 从背包中移除已使用的道具
        removeItemFromBackpack(backpack, itemIndex);
    } else {
        cout << item->name << " 不能直接使用!" << endl;
    }
}
