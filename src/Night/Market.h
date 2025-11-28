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
    
    /**
     * Constructor for MarketTrends
     * What it does: Initializes all demand values to 1.0, representing baseline demand at game start
     * Inputs: None
     * Outputs: None (constructor initializes all demand fields to 1.0)
     */
    MarketTrends();
    
    /**
     * Get demand for a specific item type
     * What it does: Maps an item name to its corresponding demand category and returns the current demand multiplier.
     *               This is used to calculate how many items a customer wants based on market trends.
     * Inputs:
     *   - itemName: Name of the item to look up
     * Outputs: Returns a float representing the current demand multiplier for the item's category.
     *          Returns food_demand as default for unknown items
     */
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
    
    /**
     * Constructor for BlackMarketItem
     * What it does: Initializes a black market item with its identifier, display name, price, and description
     * Inputs:
     *   - name: Internal item identifier used for item creation
     *   - display: Display name shown to the player in the black market menu
     *   - cost: Price in money units required to purchase this item
     *   - desc: Description text explaining what the item does
     * Outputs: None (constructor initializes member variables)
     */
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
    
    /**
     * Initialize black market stock
     * What it does: Populates the blackMarketStock vector with predefined items that players can purchase,
     *               including weapons, tools, medical supplies, and consumables
     * Inputs: None
     * Outputs: None (modifies blackMarketStock member variable)
     */
    void initializeBlackMarket();
    
public:
    /**
     * Constructor for Market class
     * What it does: Initializes a Market object with default MarketTrends and populates the black market stock
     * Inputs: None
     * Outputs: None (constructor initializes trends and calls initializeBlackMarket)
     */
    Market();
    
    /**
     * Destructor for Market class
     * What it does: Cleans up Market object resources (currently no dynamic memory to clean)
     * Inputs: None
     * Outputs: None (destructor)
     */
    ~Market();
    
    /**
     * Update market trends
     * What it does: Increases all demand values by the growth rate (e.g., 1.05 = 5% increase per day).
     *               This implements the core innovation where customer demands grow over time.
     *               Uses DEMAND_GROWTH_RATE from Config.h based on current difficulty level.
     * Inputs: None
     * Outputs: None (modifies trends member variable by multiplying each demand field by growth rate)
     */
    void updateTrends();
    
    /**
     * Get current market trends (non-const version)
     * What it does: Returns a reference to the current MarketTrends object for modification
     * Inputs: None
     * Outputs: Returns a reference to the trends member variable
     */
    MarketTrends& getTrends() { return trends; }
    
    /**
     * Get current market trends (const version)
     * What it does: Returns a const reference to the current MarketTrends object for read-only access
     * Inputs: None
     * Outputs: Returns a const reference to the trends member variable
     */
    const MarketTrends& getTrends() const { return trends; }
    
    /**
     * Set market trends
     * What it does: Replaces the current market trends with new values. Used primarily for loading saved games.
     * Inputs:
     *   - newTrends: MarketTrends object containing the new demand values to set
     * Outputs: None (modifies trends member variable)
     */
    void setTrends(const MarketTrends& newTrends);
    
    /**
     * Get black market stock
     * What it does: Returns a const reference to the vector of items available in the black market
     * Inputs: None
     * Outputs: Returns a const reference to the blackMarketStock vector
     */
    const std::vector<BlackMarketItem>& getBlackMarketStock() const { return blackMarketStock; }
    
    /**
     * Display black market menu
     * What it does: Prints a formatted menu showing all available black market items with their prices
     *               and descriptions to the console
     * Inputs: None
     * Outputs: None (prints to standard output)
     */
    void displayBlackMarket() const;
    
    /**
     * Purchase item from black market
     * What it does: Attempts to purchase an item from the black market for the player.
     *               Checks if player has sufficient funds, creates the item using ItemDatabase,
     *               deducts money from player, and adds item to player inventory.
     * Inputs:
     *   - player: Reference to Player object making the purchase
     *   - itemName: Name of the item to purchase (must match an item in black market stock)
     * Outputs: Returns true if purchase was successful, false if item not found, insufficient funds,
     *          or item creation failed
     */
    bool purchaseFromBlackMarket(Player& player, const std::string& itemName);
    
    /**
     * Get black market item by name
     * What it does: Searches the black market stock for an item matching the given name
     * Inputs:
     *   - itemName: Name of the item to search for
     * Outputs: Returns a pointer to the BlackMarketItem if found, nullptr otherwise
     */
    const BlackMarketItem* getBlackMarketItem(const std::string& itemName) const;
};

#endif // MARKET_H

