#include "Market.h"
#include "../Core/Config.h"
#include "../Item/itemdatabase.h"
#include "../Item/item.h"
#include "../Player/Player.h"
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>

/**
 * MarketTrends constructor
 * What it does: Initializes all demand values (food, water, medicine, metal, tool) to 1.0,
 *               representing baseline demand at the start of the game
 * Inputs: None
 * Outputs: None (constructor initializes all demand fields to 1.0)
 */
MarketTrends::MarketTrends() 
    : food_demand(1.0f), water_demand(1.0f), medicine_demand(1.0f), 
      metal_demand(1.0f), tool_demand(1.0f) {
}

/**
 * Get demand for a specific item type
 * What it does: Maps an item name to its corresponding demand category and returns the current
 *               demand multiplier. Items are categorized into food, medicine, metal, or tool demands.
 * Inputs:
 *   - itemName: Name of the item to look up (e.g., "Canned food", "Bandage", "Scrap Metal")
 * Outputs: Returns a float representing the current demand multiplier for the item's category.
 *          Returns food_demand as default for unknown items
 */
float MarketTrends::getDemandForItem(const std::string& itemName) const {
    // Map item names to demand types
    if (itemName == "Canned food" || itemName == "Military Rations" || itemName == "Water") {
        return food_demand;
    } else if (itemName == "Bandage" || itemName == "First Aid Kit") {
        return medicine_demand;
    } else if (itemName == "Scrap Metal" || itemName == "Electronic Component" || 
               itemName == "Gold Ring" || itemName == "Rag") {
        return metal_demand;
    } else if (itemName == "Crowbar" || itemName == "Lock pick") {
        return tool_demand;
    }
    // Default to food demand for unknown items
    return food_demand;
}

/**
 * BlackMarketItem constructor
 * What it does: Initializes a BlackMarketItem with the provided name, display name, price, and description
 * Inputs:
 *   - name: Internal item identifier used for item creation
 *   - display: Display name shown to players
 *   - cost: Purchase price in money units
 *   - desc: Item description text
 * Outputs: None (constructor initializes member variables)
 */
BlackMarketItem::BlackMarketItem(const std::string& name, const std::string& display, 
                                 int cost, const std::string& desc)
    : item_name(name), display_name(display), price(cost), description(desc) {
}

/**
 * Market constructor
 * What it does: Initializes a Market object by creating default MarketTrends and populating
 *               the black market stock with available items
 * Inputs: None
 * Outputs: None (constructor initializes trends and calls initializeBlackMarket)
 */
Market::Market() {
    trends = MarketTrends();
    initializeBlackMarket();
}

/**
 * Market destructor
 * What it does: Cleans up Market object. Currently no dynamic memory to deallocate.
 * Inputs: None
 * Outputs: None (destructor)
 */
Market::~Market() {
    // No dynamic memory to clean up
}

/**
 * Initialize black market stock
 * What it does: Populates the blackMarketStock vector with predefined items that players can purchase
 *               from the black market merchant, including weapons, tools, medical supplies, and consumables
 * Inputs: None
 * Outputs: None (modifies blackMarketStock member variable by adding BlackMarketItem objects)
 */
void Market::initializeBlackMarket() {
    blackMarketStock.clear();
    
    // Add items available from black market
    // These should match items from ItemDatabase that are sold in the store
    blackMarketStock.push_back(BlackMarketItem(
        "Military Rations", "Military Rations", 25,
        "High-quality food that restores 40 HP and 25 Stamina"
    ));
    
    blackMarketStock.push_back(BlackMarketItem(
        "Bandage", "Bandage", 5,
        "Basic medical supplies that restore 15 HP"
    ));
    
    blackMarketStock.push_back(BlackMarketItem(
        "First Aid Kit", "First Aid Kit", 30,
        "Advanced medical supplies that restore 50 HP"
    ));
    
    blackMarketStock.push_back(BlackMarketItem(
        "Crowbar", "Crowbar", 25,
        "Tool for opening sealed containers (20 durability)"
    ));
    
    blackMarketStock.push_back(BlackMarketItem(
        "Lock pick", "Lock pick", 45,
        "Professional lock-picking tool (15 durability)"
    ));
    
    blackMarketStock.push_back(BlackMarketItem(
        "Baseball Bat", "Baseball Bat", 20,
        "Reliable melee weapon (15 damage, 50 durability)"
    ));
    
    blackMarketStock.push_back(BlackMarketItem(
        "Shotgun", "Shotgun", 80,
        "Reliable ranged weapon (45 damage, 30 durability)"
    ));
    
    blackMarketStock.push_back(BlackMarketItem(
        "Bullet", "Bullet", 2,
        "Shotgun ammunition"
    ));
}

/**
 * Update market trends
 * What it does: Increases all demand values (food, water, medicine, metal, tool) by the growth rate
 *               determined by the current game difficulty. This implements the core innovation where
 *               customer demands grow over time (e.g., 5% per day). The growth rate is retrieved from
 *               Config.h based on difficulty level (Easy, Normal, Hard).
 * Inputs: None
 * Outputs: None (modifies trends member variable by multiplying each demand field by the appropriate growth rate)
 */
void Market::updateTrends() {
    // Use DEMAND_GROWTH_RATE from Config.h based on current difficulty
    float growthRate = NormalConfig::DEMAND_GROWTH_RATE;  // Default to normal
    
    // Get current difficulty from itemdatabase
    int difficulty = getGameDifficulty();
    switch (difficulty) {
        case DIFFICULTY_EASY:
            growthRate = EasyConfig::DEMAND_GROWTH_RATE;
            break;
        case DIFFICULTY_NORMAL:
            growthRate = NormalConfig::DEMAND_GROWTH_RATE;
            break;
        case DIFFICULTY_HARD:
            growthRate = HardConfig::DEMAND_GROWTH_RATE;
            break;
        default:
            growthRate = NormalConfig::DEMAND_GROWTH_RATE;
            break;
    }
    
    trends.food_demand *= growthRate;
    trends.water_demand *= growthRate;
    trends.medicine_demand *= growthRate;
    trends.metal_demand *= growthRate;
    trends.tool_demand *= growthRate;
}

/**
 * Set market trends
 * What it does: Replaces the current market trends with new values. Primarily used when loading
 *               a saved game to restore the market state from a previous session.
 * Inputs:
 *   - newTrends: MarketTrends object containing the demand values to restore
 * Outputs: None (modifies trends member variable by assignment)
 */
void Market::setTrends(const MarketTrends& newTrends) {
    trends = newTrends;
}

/**
 * Display black market menu
 * What it does: Prints a formatted menu to the console showing all available black market items.
 *               For each item, displays its number, display name, price, and description.
 *               Also shows option [0] to return to the previous menu.
 * Inputs: None
 * Outputs: None (prints formatted menu to standard output)
 */
void Market::displayBlackMarket() const {
    std::cout << "\n=== Black Market Merchant ===" << std::endl;
    std::cout << "Welcome to the black market! Here are all the items you need.\n" << std::endl;
    
    for (size_t i = 0; i < blackMarketStock.size(); i++) {
        const BlackMarketItem& item = blackMarketStock[i];
        std::cout << "[" << (i + 1) << "] " << item.display_name 
                  << " - " << item.price << " Money" << std::endl;
        std::cout << "    " << item.description << std::endl;
    }
    std::cout << "[0] Return" << std::endl;
}

/**
 * Purchase item from black market
 * What it does: Processes a purchase transaction for the player. First verifies the item exists in
 *               the black market stock, then checks if the player has sufficient funds. If both checks
 *               pass, creates the item using ItemDatabase::createItem(), deducts the price from player's
 *               money, adds the item to player's inventory, and displays confirmation messages.
 * Inputs:
 *   - player: Reference to Player object making the purchase
 *   - itemName: Name of the item to purchase (must match item_name in blackMarketStock)
 * Outputs: Returns true if purchase was successful (item found, sufficient funds, item created).
 *          Returns false if item not found, insufficient funds, or item creation failed.
 *          Prints error messages to console on failure.
 */
bool Market::purchaseFromBlackMarket(Player& player, const std::string& itemName) {
    const BlackMarketItem* item = getBlackMarketItem(itemName);
    if (!item) {
        std::cout << "Error: Item not found" << std::endl;
        return false;
    }
    
    // Check if player has enough money
    if (player.money < item->price) {
        std::cout << "Insufficient funds!" << std::endl;
        std::cout << "Need: " << item->price << " Money" << std::endl;
        std::cout << "Have: " << player.money << " Money" << std::endl;
        return false;
    }
    
    // Create the item using ItemDatabase
    Item* newItem = createItem(itemName);
    if (!newItem) {
        std::cout << "Error: Cannot create item" << std::endl;
        return false;
    }
    
    // Deduct money
    player.money -= item->price;
    
    // Add item to inventory
    player.addItem(newItem);
    
    std::cout << "Purchase successful!" << std::endl;
    std::cout << "Obtained: " << item->display_name << std::endl;
    std::cout << "Remaining money: " << player.money << std::endl;
    
    return true;
}

/**
 * Get black market item by name
 * What it does: Searches through the black market stock vector to find an item matching the given name.
 *               Uses linear search to compare item_name fields.
 * Inputs:
 *   - itemName: Name of the item to search for (must match item_name exactly)
 * Outputs: Returns a pointer to the BlackMarketItem if found, nullptr if no matching item exists
 */
const BlackMarketItem* Market::getBlackMarketItem(const std::string& itemName) const {
    for (const auto& item : blackMarketStock) {
        if (item.item_name == itemName) {
            return &item;
        }
    }
    return nullptr;
}

