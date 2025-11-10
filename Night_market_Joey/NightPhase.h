#ifndef NIGHTPHASE_H
#define NIGHTPHASE_H

#include <vector>
#include <string>
#include "Customer.h"  // For Customer struct definition
#include "Market.h"    // For Market class definition

// Forward declarations
class Player;

/**
 * NightPhase class handling all night phase operations
 * Manages customer trading, black market access, and end-of-day updates
 */
class NightPhase {
private:
    Player* m_player;      // Pointer to player
    Market* m_market;      // Pointer to market
    
    /**
     * Display customer list and their demands
     * @param customers Vector of customers to display
     */
    void displayCustomers(const std::vector<Customer>& customers) const;
    
    /**
     * Handle trading with a specific customer
     * @param customer Customer to trade with
     * @return True if trade was successful, false otherwise
     */
    bool handleCustomerTrade(const Customer& customer);
    
    /**
     * Calculate final price based on customer patience and base price
     * @param customer Customer making the purchase
     * @param quantity Quantity being sold
     * @return Final price for the transaction
     */
    int calculateTradePrice(const Customer& customer, int quantity) const;
    
    /**
     * Check if player has enough items to sell
     * @param itemName Name of the item
     * @param quantity Required quantity
     * @return True if player has enough items
     */
    bool playerHasItem(const std::string& itemName, int quantity) const;
    
    /**
     * Display night phase menu
     */
    void displayMenu() const;
    
    /**
     * Handle black market access
     */
    void handleBlackMarket();
    
    /**
     * Process end of night phase (update market trends, charge fees, etc.)
     * @param day Current day number
     * @return True if player can continue, false if game over (insufficient funds)
     */
    bool processEndOfNight(int day);
    
public:
    // Constructor
    NightPhase(Player* player, Market* market);
    
    // Default constructor (for static usage)
    NightPhase();
    
    /**
     * Execute the night phase
     * This is the main entry point for night phase
     * @param player Reference to player
     * @param market Reference to market
     */
    static void executeNight(Player& player, Market& market);
    
    /**
     * Execute the night phase (instance method)
     * @param day Current day number
     */
    void executeNightPhase(int day);
    
    /**
     * Set player and market pointers
     */
    void setPlayer(Player* player) { m_player = player; }
    void setMarket(Market* market) { m_market = market; }
};

#endif // NIGHTPHASE_H

