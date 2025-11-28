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
    /**
     * @brief Serialize player data to file
     * @param file Input: Output file stream to write to
     * @param player Input: Player object to serialize
     * @return void
     */
    static void serializePlayer(std::ofstream& file, Player& player);     

    /**
     * @brief Deserialize player data from file
     * @param file Input: Input file stream to read from
     * @param player Output: Player object to populate with deserialized data
     * @return bool: True if deserialization succeeded, false otherwise
     */
    static bool deserializePlayer(std::ifstream& file, Player& player);  
    
    // Inventory serialization
    /**
     * @brief Serialize inventory data to file
     * @param file Input: Output file stream to write to
     * @param inventory Input: Inventory vector to serialize
     * @return void
     */
    static void serializeInventory(std::ofstream& file, std::vector<Item*>& inventory);     

    /**
     * @brief Deserialize inventory data from file
     * @param file Input: Input file stream to read from
     * @param player Output: Player object to populate with deserialized inventory
     * @return bool: True if deserialization succeeded, false otherwise
     */
    static bool deserializeInventory(std::ifstream& file, Player& player);                  

    // MarketTrends serialization - temporarily removed

public:
    /**
     * @brief Save current game state to file
     * @param player Input: Player object to save
     * @param day Input: Current game day to save
     * @return void
     */
    static void saveGame(Player& player, int day);

    /**
     * @brief Load game state from file
     * @param player Output: Player object to populate with loaded data
     * @param day Output: Variable to store loaded game day
     * @return bool: True if load succeeded, false otherwise
     */
    static bool loadGame(Player& player, int& day);
    
    /**
     * @brief Check if save file exists
     * @return bool: True if save file exists, false otherwise
     */
    static bool saveExists();
    
    /**
     * @brief Delete existing save file
     * @return void
     */
    static void deleteSave();
    
    // Demand growth function - temporarily removed
};

#endif
