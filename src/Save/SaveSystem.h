#ifndef SAVESYSTEM_H
#define SAVESYSTEM_H

#include "../Core/Common.h"
#include "../Player/Player.h"
#include "../Item/item.h"
#include "../Item/itemdatabase.h"
#include "../Core/Config.h"
#include <string>
#include <fstream>
#include <vector>

// MarketTrends struct - defined in Market.h

class SaveSystem {
private:
    static std::string saveFileName;  // Static save file name

    // Internal serialization methods
    static void serializePlayer(std::ofstream& file, Player& player);     // Serialize player data
    static bool deserializePlayer(std::ifstream& file, Player& player);  // Deserialize player data
    
    // Inventory serialization
    static void serializeInventory(std::ofstream& file, std::vector<Item*>& inventory);     // Serialize inventory data
    static bool deserializeInventory(std::ifstream& file, Player& player);                  // Deserialize inventory data
    
    // MarketTrends serialization - temporarily removed

public:
    // Save game state
    static void saveGame(Player& player, int day);

    // Load game state
    static bool loadGame(Player& player, int& day);
    
    // Check if save file exists
    static bool saveExists();
    
    // Delete save file
    static void deleteSave();
    
    // Demand growth function - temporarily removed
};

#endif
