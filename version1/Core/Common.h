
#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <vector>
using namespace std;

enum GameState {
    MAIN_MENU,
    DAY,
    NIGHT,
    COMBAT,
    GAME_OVER
};

enum ItemType { 
    SCRAP,  
    WEAPON,
    FOOD,
    MEDICINE,
    TOOL,
    AMMO
};

struct Item;

#endif 