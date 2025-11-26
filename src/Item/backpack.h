#ifndef BACKPACK_H
#define BACKPACK_H

#include "item.h"

// 背包结构体
struct Backpack {
    string name;
    int capacity;
    Item* items[50];  // 使用指针数组
    int itemCount;
};

// 背包操作函数声明
void initBackpack(Backpack &backpack, string name, int capacity);
bool addItemToBackpack(Backpack &backpack, Item* item);
void removeItemFromBackpack(Backpack &backpack, int index);
void showBackpackContents(Backpack &backpack);
int calculateBackpackTotalValue(Backpack &backpack);
void useItemFromBackpack(Backpack &backpack, int itemIndex);

#endif
