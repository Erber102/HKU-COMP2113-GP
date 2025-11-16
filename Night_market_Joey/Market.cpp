#include "Market.h"
#include "Config.h"
#include "itemdatabase.h"
#include "item.h"
#include "../Player.h"
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>

// MarketTrends constructor
MarketTrends::MarketTrends() 
    : food_demand(1.0f), water_demand(1.0f), medicine_demand(1.0f), 
      metal_demand(1.0f), tool_demand(1.0f) {
}

// Get demand for a specific item type
float MarketTrends::getDemandForItem(const std::string& itemName) const {
    // Map item names to demand types
    if (itemName == "Canned food" || itemName == "Military Rations") {
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

// BlackMarketItem constructor
BlackMarketItem::BlackMarketItem(const std::string& name, const std::string& display, 
                                 int cost, const std::string& desc)
    : item_name(name), display_name(display), price(cost), description(desc) {
}

// Market constructor
Market::Market() {
    trends = MarketTrends();
    initializeBlackMarket();
}

// Market destructor
Market::~Market() {
    // No dynamic memory to clean up
}

// Initialize black market stock
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

// Update market trends
void Market::updateTrends() {
    // Use DEMAND_GROWTH_RATE from Config.h
    trends.food_demand *= DEMAND_GROWTH_RATE;
    trends.water_demand *= DEMAND_GROWTH_RATE;
    trends.medicine_demand *= DEMAND_GROWTH_RATE;
    trends.metal_demand *= DEMAND_GROWTH_RATE;
    trends.tool_demand *= DEMAND_GROWTH_RATE;
}

// Set market trends (for loading saved games)
void Market::setTrends(const MarketTrends& newTrends) {
    trends = newTrends;
}

// Display black market menu
void Market::displayBlackMarket() const {
    std::cout << "\n=== 黑市商人 (Black Market Merchant) ===" << std::endl;
    std::cout << "欢迎来到黑市！这里有你需要的所有道具。" << std::endl;
    std::cout << "Welcome to the black market! Here are all the items you need.\n" << std::endl;
    
    for (size_t i = 0; i < blackMarketStock.size(); i++) {
        const BlackMarketItem& item = blackMarketStock[i];
        std::cout << "[" << (i + 1) << "] " << item.display_name 
                  << " - " << item.price << " 货币 (Money)" << std::endl;
        std::cout << "    " << item.description << std::endl;
    }
    std::cout << "[0] 返回 (Return)" << std::endl;
}

// Purchase item from black market
bool Market::purchaseFromBlackMarket(Player& player, const std::string& itemName) {
    const BlackMarketItem* item = getBlackMarketItem(itemName);
    if (!item) {
        std::cout << "错误：找不到该物品 (Error: Item not found)" << std::endl;
        return false;
    }
    
    // Check if player has enough money
    if (player.getMoney() < item->price) {
        std::cout << "金钱不足！(Insufficient funds!)" << std::endl;
        std::cout << "需要 (Need): " << item->price << " 货币 (Money)" << std::endl;
        std::cout << "拥有 (Have): " << player.getMoney() << " 货币 (Money)" << std::endl;
        return false;
    }
    
    // Create the item using ItemDatabase
    Item* newItem = createItem(itemName);
    if (!newItem) {
        std::cout << "错误：无法创建物品 (Error: Cannot create item)" << std::endl;
        return false;
    }
    
    // Deduct money
    player.setMoney(player.getMoney() - item->price);
    
    // Add item to inventory
    player.addItem(newItem);
    
    std::cout << "购买成功！(Purchase successful!)" << std::endl;
    std::cout << "获得 (Obtained): " << item->display_name << std::endl;
    std::cout << "剩余金钱 (Remaining money): " << player.getMoney() << std::endl;
    
    return true;
}

// Get black market item by name
const BlackMarketItem* Market::getBlackMarketItem(const std::string& itemName) const {
    for (const auto& item : blackMarketStock) {
        if (item.item_name == itemName) {
            return &item;
        }
    }
    return nullptr;
}

