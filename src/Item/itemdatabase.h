#ifndef ITEMDATABASE_H
#define ITEMDATABASE_H

#include "item.h"
#include <memory>
#include <string>

void createItemPrototype(int id, std::string name, std::string desc, int cat, int val, int health, int stamina, int dmg, int dur, bool edible);
void initItemDatabase();
void setGameDifficulty(int difficulty);
std::unique_ptr<Item> createItem(std::string itemName);
int getGameDifficulty();
std::unique_ptr<Item> getRandomScrapWithDifficulty(int difficulty);

std::unique_ptr<Item> getShopItem(int index);
int getShopItemCount();
std::unique_ptr<Item> getMostExpensiveItem();
std::unique_ptr<Item> getCheapestItem();
bool isValidItem(Item* item);
std::unique_ptr<Item> findItemById(int id);

#endif
