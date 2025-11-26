#ifndef MARKET_H
#define MARKET_H

#include <string>
#include <vector>
#include <map>

// Forward declarations
struct Item;
class Player;

// Note: Config.h should define DEMAND_GROWTH_RATE constant
// For now, we'll use a default value if Config.h doesn't exist
// The implementation in Market.cpp should check for Config.h and use DEMAND_GROWTH_RATE if available

/**
 * MarketTrends structure to track global demand trends
 * Demand grows over time, affecting customer purchase quantities
 */
struct MarketTrends {
    float food_demand;        // Demand for food items (initial 1.0)
    float water_demand;       // Demand for water items (initial 1.0)
    float medicine_demand;    // Demand for medicine items (initial 1.0)
    float metal_demand;       // Demand for metal/scrap items (initial 1.0)
    float tool_demand;        // Demand for tools (initial 1.0)
    
    // Constructor - initialize all demands to 1.0
    MarketTrends();
    
    // Get demand for a specific item type
    float getDemandForItem(const std::string& itemName) const;
};

/**
 * Black market merchant item entry
 * Represents an item that can be purchased from the black market
 */
struct BlackMarketItem {
    std::string item_name;    // Item identifier
    std::string display_name; // Display name for the item
    int price;                // Price to purchase
    std::string description;  // Item description
    
    BlackMarketItem(const std::string& name, const std::string& display, 
                    int cost, const std::string& desc);
};

/**
 * Market class managing market trends and black market merchant
 */
class Market {
private:
    MarketTrends trends;                           // Current market trends
    std::vector<BlackMarketItem> blackMarketStock; // Items available from black market
    
    // Initialize black market stock
    void initializeBlackMarket();
    
public:
    // Constructor
    Market();
    
    // Destructor
    ~Market();
    
    // Update market trends (called at end of each night)
    // Increases demand by growth rate (e.g., 1.05 = 5% increase)
    // Uses DEMAND_GROWTH_RATE from Config.h if available, otherwise uses default
    void updateTrends();
    
    // Get current market trends
    MarketTrends& getTrends() { return trends; }
    const MarketTrends& getTrends() const { return trends; }
    
    // Set market trends (used for loading saved games)
    void setTrends(const MarketTrends& newTrends);
    
    // Get black market stock
    const std::vector<BlackMarketItem>& getBlackMarketStock() const { return blackMarketStock; }
    
    // Display black market menu
    void displayBlackMarket() const;
    
    // Purchase item from black market
    // Returns true if purchase successful, false otherwise
    bool purchaseFromBlackMarket(Player& player, const std::string& itemName);
    
    // Get black market item by name
    const BlackMarketItem* getBlackMarketItem(const std::string& itemName) const;
};

#endif // MARKET_H

