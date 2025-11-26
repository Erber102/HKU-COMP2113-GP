#ifndef ITEM_H
#define ITEM_H

#include <iostream>
#include <string>
using namespace std;

// Item category constants
const int CATEGORY_SCRAP = 0;    // Scrap
const int CATEGORY_FOOD = 1;     // Food (Edible)
const int CATEGORY_MEDICINE = 2; // Medical
const int CATEGORY_TOOL = 3;     // Tools
const int CATEGORY_WEAPON = 4;   // Weapons
const int CATEGORY_AMMO = 5;     // Ammunition

// difficulty level constants - defined in Config.h

// Item structure
struct Item{
    int id;
    string name;
    string description;
    int category;
    int value;
    int healthEffect;
    int staminaEffect;
    int damage;
    int durability;
    bool isEdible;
};

void showItemInfo(Item item);

#endif
