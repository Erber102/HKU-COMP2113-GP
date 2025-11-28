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
     * What it does: Prints a formatted list of all customers showing their name, desired item and quantity,
     *               offered price, and patience level to the console
     * Inputs:
     *   - customers: Vector of Customer objects to display
     * Outputs: None (prints formatted customer information to standard output)
     */
    void displayCustomers(const std::vector<Customer>& customers) const;
    
    /**
     * Handle trading with a specific customer
     * What it does: Processes a trade transaction with a customer. Displays trade details, checks if
     *               player has sufficient items, calculates total price, prompts for confirmation,
     *               removes items from player inventory, and adds money to player if trade is accepted.
     * Inputs:
     *   - customer: Customer object representing the NPC to trade with
     * Outputs: Returns true if trade was successful (player had items and confirmed trade).
     *          Returns false if player lacks sufficient items or cancelled the trade.
     *          Prints trade information and results to console.
     */
    bool handleCustomerTrade(const Customer& customer);
    
    /**
     * Calculate final price based on customer patience and base price
     * What it does: Calculates the total price for a trade by multiplying base price per item by quantity,
     *               then applies a multiplier based on customer patience (higher patience = better price)
     * Inputs:
     *   - customer: Customer object containing base_price and patience values
     *   - quantity: Number of items being sold in the transaction
     * Outputs: Returns an integer representing the final total price for the transaction
     */
    int calculateTradePrice(const Customer& customer, int quantity) const;
    
    /**
     * Check if player has enough items to sell
     * What it does: Verifies whether the player's inventory contains at least the required quantity
     *               of the specified item by calling Player::getItemCount()
     * Inputs:
     *   - itemName: Name of the item to check for
     *   - quantity: Minimum quantity required
     * Outputs: Returns true if player has at least the required quantity of the item.
     *          Returns false if player has insufficient items or player pointer is null
     */
    bool playerHasItem(const std::string& itemName, int quantity) const;
    
    /**
     * Display night phase menu
     * What it does: Prints the main menu options for the night phase to the console,
     *               including options to view customers, trade, access black market, view inventory,
     *               end night, and save & exit
     * Inputs: None
     * Outputs: None (prints menu to standard output)
     */
    void displayMenu() const;
    
    /**
     * Handle black market access
     * What it does: Manages the black market interaction loop. Displays black market menu,
     *               processes player input for item selection, prompts for purchase confirmation,
     *               and calls Market::purchaseFromBlackMarket() to complete transactions.
     *               Continues until player selects option to return.
     * Inputs: None
     * Outputs: None (modifies player inventory and money through Market::purchaseFromBlackMarket())
     */
    void handleBlackMarket();
    
    /**
     * Process end of night phase
     * What it does: Handles end-of-night operations including updating market trends (demand growth),
     *               displaying updated demand values, charging the daily fee, and checking if player
     *               has sufficient funds to continue. Implements game over condition if funds insufficient.
     * Inputs:
     *   - day: Current day number (used for display purposes)
     * Outputs: Returns true if player can continue to next day (sufficient funds for daily fee).
     *          Returns false if game over (insufficient funds). Modifies market trends and player money.
     */
    bool processEndOfNight(int day);
    
public:
    /**
     * Constructor for NightPhase
     * What it does: Initializes a NightPhase object with pointers to Player and Market objects
     * Inputs:
     *   - player: Pointer to Player object
     *   - market: Pointer to Market object
     * Outputs: None (constructor initializes member pointers)
     */
    NightPhase(Player* player, Market* market);
    
    /**
     * Default constructor for NightPhase
     * What it does: Creates a NightPhase object with null pointers. Used for static method calls.
     * Inputs: None
     * Outputs: None (constructor initializes member pointers to nullptr)
     */
    NightPhase();
    
    /**
     * Execute the night phase (static method)
     * What it does: Main entry point for the night phase. Creates a temporary Market if none provided,
     *               instantiates NightPhase, and calls executeNightPhase() to run the night phase loop.
     *               This static method allows calling without creating a NightPhase instance first.
     * Inputs:
     *   - player: Reference to Player object
     *   - market: Pointer to Market object (can be nullptr, will create temporary Market if needed)
     *   - day: Current day number
     * Outputs: None (calls executeNightPhase() which handles the night phase logic)
     */
    static void executeNight(Player& player, Market* market, int day);
    
    /**
     * Execute the night phase (instance method)
     * What it does: Runs the main night phase game loop. Generates customers, displays menu,
     *               processes player choices (view customers, trade, black market, inventory, end night, save),
     *               handles input validation, and manages the night phase state until player ends night
     *               or triggers game over condition.
     * Inputs:
     *   - day: Current day number (used for display and passed to processEndOfNight())
     * Outputs: None (modifies player state through trades and purchases, may exit game on save & exit)
     */
    void executeNightPhase(int day);
    
    /**
     * Set player pointer
     * What it does: Updates the internal player pointer to point to the provided Player object
     * Inputs:
     *   - player: Pointer to Player object
     * Outputs: None (modifies m_player member variable)
     */
    void setPlayer(Player* player) { m_player = player; }
    
    /**
     * Set market pointer
     * What it does: Updates the internal market pointer to point to the provided Market object
     * Inputs:
     *   - market: Pointer to Market object
     * Outputs: None (modifies m_market member variable)
     */
    void setMarket(Market* market) { m_market = market; }
};

#endif // NIGHTPHASE_H

