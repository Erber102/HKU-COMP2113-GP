#ifndef ITEMDATABASE_H
#define ITEMDATABASE_H

#include "Item.h"

void createItemPrototype(int id, string name, string desc, int cat, int val, int health, int stamina, int dmg, int dur, bool edible);
void initItemDatabase();
void setGameDifficulty(int difficulty);
Item* createItem(string itemName);
int getGameDifficulty();
Item* getRandomScrapWithDifficulty(int difficulty);
void deleteItem(Item* item);

#endif