#ifndef ITEMDATABASE_H
#define ITEMDATABASE_H

#include "item.h"
#include <string>
#include "../Core/Config.h"


using namespace std;

void createItemPrototype(int id, string name, string desc, int cat, int val, int health, int stamina, int dmg, int dur, bool edible);
void initItemDatabase();
void setGameDifficulty(int difficulty);
Item* createItem(string itemName);
int getGameDifficulty();
Item* getRandomScrapWithDifficulty(int difficulty);
void deleteItem(Item* item);

Item* getShopItem(int index);
int getShopItemCount();
Item* getMostExpensiveItem();
Item* getCheapestItem();
bool isValidItem(Item* item);


Item* findItemById(int id);



#endif
