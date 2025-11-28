#include "NightPhase.h"
#include "Customer.h"
#include "Market.h"
#include "../Core/Config.h"
#include "../Item/item.h"
#include "../Item/itemdatabase.h"
#include "../Player/Player.h"
#include "../Save/SaveSystem.h"
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <limits>

/**
 * Constructor for NightPhase
 * What it does: Initializes a NightPhase object with pointers to Player and Market objects
 * Inputs:
 *   - player: Pointer to Player object
 *   - market: Pointer to Market object
 * Outputs: None (constructor initializes m_player and m_market member variables)
 */
NightPhase::NightPhase(Player* player, Market* market) 
    : m_player(player), m_market(market) {
}

/**
 * Default constructor for NightPhase
 * What it does: Creates a NightPhase object with null pointers for player and market.
 *               Used when calling static methods without an instance.
 * Inputs: None
 * Outputs: None (constructor initializes m_player and m_market to nullptr)
 */
NightPhase::NightPhase() 
    : m_player(nullptr), m_market(nullptr) {
}

/**
 * Execute the night phase (static method)
 * What it does: Main entry point for the night phase. If market pointer is null, creates a temporary
 *               static Market object. Then creates a NightPhase instance and calls executeNightPhase()
 *               to run the night phase game loop.
 * Inputs:
 *   - player: Reference to Player object
 *   - market: Pointer to Market object (can be nullptr, will use temporary Market if needed)
 *   - day: Current day number
 * Outputs: None (delegates to executeNightPhase() which handles all night phase logic)
 */
void NightPhase::executeNight(Player& player, Market* market, int day) {
    if (!market) {
        // If market is nullptr, create a temporary one
        // In actual game, market should be properly initialized
        static Market tempMarket;
        market = &tempMarket;
    }
    
    NightPhase nightPhase(&player, market);
    nightPhase.executeNightPhase(day);
}

/**
 * Execute the night phase (instance method)
 * What it does: Runs the main night phase game loop. Validates player and market pointers, generates
 *               customers for the night, displays welcome message, then enters a loop that displays menu,
 *               processes player input, handles menu choices (view customers, trade, black market, inventory,
 *               end night, save & exit), validates input, and manages night phase state. Continues until
 *               player ends night or triggers game over.
 * Inputs:
 *   - day: Current day number (displayed in welcome message and passed to processEndOfNight())
 * Outputs: None (modifies player state through trades and purchases, may exit program on save & exit,
 *          may trigger game over if insufficient funds)
 */
void NightPhase::executeNightPhase(int day) {
    if (!m_player || !m_market) {
        std::cout << "Error: Player or Market not initialized" << std::endl;
        return;
    }
    
    std::cout << "\n=== Night Phase - Day " << day << " ===" << std::endl;
    std::cout << "Welcome back to your safe house!" << std::endl;
    
    // Generate customers
    MarketTrends& trends = m_market->getTrends();
    std::vector<Customer> customers = generateCustomers(trends);
    
    bool nightPhaseActive = true;
    int invalidInputCount = 0;
    const int MAX_INVALID_INPUTS = 5;

    while (nightPhaseActive) {
        displayMenu();

        int choice;
        std::cout << "\nPlease choose: ";

        // Input validation
        if (!(std::cin >> choice)) {
            std::cin.clear(); // Clear error state
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore bad input
            std::cout << "Invalid input! Please enter a number." << std::endl;
            invalidInputCount++;

            if (invalidInputCount >= MAX_INVALID_INPUTS) {
                std::cout << "Too many invalid inputs. Ending night phase." << std::endl;
                break;
            }
            continue;
        }

        switch (choice) {
            case 1:
                // View customers
                displayCustomers(customers);
                invalidInputCount = 0; // Reset invalid input count
                break;

            case 2:
                // Trade with customer
                if (customers.empty()) {
                    std::cout << "No customers" << std::endl;
                } else {
                    std::cout << "\nSelect customer to trade with:" << std::endl;
                    for (size_t i = 0; i < customers.size(); i++) {
                        std::cout << "[" << (i + 1) << "] " << customers[i].name << std::endl;
                    }
                    std::cout << "[0] Return" << std::endl;

                    int customerChoice;
                    std::cout << "Choice: ";

                    if (!(std::cin >> customerChoice)) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "Invalid input!" << std::endl;
                    } else if (customerChoice > 0 && customerChoice <= static_cast<int>(customers.size())) {
                        handleCustomerTrade(customers[customerChoice - 1]);
                    }
                }
                invalidInputCount = 0;
                break;

            case 3:
                // Black market
                handleBlackMarket();
                invalidInputCount = 0;
                break;

            case 4:
                // View inventory
                m_player->displayInventory();
                invalidInputCount = 0;
                break;

            case 5:
                // End night phase
                if (processEndOfNight(day)) {
                    nightPhaseActive = false;
                } else {
                    std::cout << "Game Over! Insufficient funds to pay fee" << std::endl;
                    return;
                }
                break;

            case 6:
                SaveSystem::saveGame(*m_player, day);
                std::cout << "Game saved. Exiting..." << std::endl;
                std::exit(0);
                break;

            default:
                std::cout << "Invalid choice" << std::endl;
                invalidInputCount++;

                if (invalidInputCount >= MAX_INVALID_INPUTS) {
                    std::cout << "Too many invalid inputs. Ending night phase." << std::endl;
                    nightPhaseActive = false;
                }
                break;
        }
    }
}

/**
 * Display customer list and their demands
 * What it does: Prints a formatted list of all customers to the console, showing each customer's
 *               number, name, desired item and quantity, offered total price, and patience level.
 *               If no customers exist, displays "No customers" message.
 * Inputs:
 *   - customers: Vector of Customer objects to display
 * Outputs: None (prints formatted customer information to standard output)
 */
void NightPhase::displayCustomers(const std::vector<Customer>& customers) const {
    std::cout << "\n=== Customer List ===" << std::endl;
    
    if (customers.empty()) {
        std::cout << "No customers" << std::endl;
        return;
    }
    
    for (size_t i = 0; i < customers.size(); i++) {
        const Customer& c = customers[i];
        std::cout << "\n[" << (i + 1) << "] " << c.name << std::endl;
        std::cout << "   Wants to buy: " << c.desired_item 
                  << " x" << c.desired_quantity << std::endl;
        std::cout << "   Offers: " << calculateTradePrice(c, c.desired_quantity) 
                  << " Money" << std::endl;
        std::cout << "   Patience: " << c.patience << std::endl;
    }
}

/**
 * Handle trading with a specific customer
 * What it does: Processes a complete trade transaction with a customer. Displays trade details
 *               (customer name, desired item and quantity), checks if player has sufficient items,
 *               calculates and displays total price, prompts player for confirmation, removes items
 *               from player inventory if confirmed, adds money to player, and displays success message.
 * Inputs:
 *   - customer: Customer object containing name, desired_item, desired_quantity, patience, and base_price
 * Outputs: Returns true if trade was successful (player had items and confirmed trade).
 *          Returns false if player lacks sufficient items or cancelled the trade.
 *          Modifies player inventory (removes items) and player money (adds payment).
 *          Prints trade information and results to console.
 */
bool NightPhase::handleCustomerTrade(const Customer& customer) {
    std::cout << "\n=== Trade ===" << std::endl;
    std::cout << "Customer: " << customer.name << std::endl;
    std::cout << "Wants to buy: " << customer.desired_item 
              << " x" << customer.desired_quantity << std::endl;
    
    // Check if player has the item
    if (!playerHasItem(customer.desired_item, customer.desired_quantity)) {
        std::cout << "You don't have enough items!" << std::endl;
        return false;
    }
    
    int totalPrice = calculateTradePrice(customer, customer.desired_quantity);
    std::cout << "Total price: " << totalPrice << " Money" << std::endl;
    
    std::cout << "\nAgree to trade? [1] Yes [2] No: ";
    int choice;
    std::cin >> choice;
    
    if (choice != 1) {
        std::cout << "Trade cancelled" << std::endl;
        return false;
    }
    
    // Remove items from player inventory
    std::string itemName = customer.desired_item;
    m_player->removeItem(itemName, customer.desired_quantity);
    
    // Add money to player
    m_player->money += totalPrice;
    
    std::cout << "Trade successful!" << std::endl;
    std::cout << "Earned: " << totalPrice << " Money" << std::endl;
    
    return true;
}

/**
 * Calculate final price based on customer patience and base price
 * What it does: Calculates the total transaction price by multiplying customer's base price per item
 *               by the quantity, then applies a patience-based multiplier. Higher patience results
 *               in a better price (patience multiplier = 1.0 + (patience - 1) * 0.1).
 * Inputs:
 *   - customer: Customer object containing base_price and patience values
 *   - quantity: Number of items being sold in the transaction
 * Outputs: Returns an integer representing the final total price for the transaction after applying
 *          patience multiplier
 */
int NightPhase::calculateTradePrice(const Customer& customer, int quantity) const {
    // Base price calculation
    int basePrice = customer.base_price * quantity;
    
    // Adjust based on patience (higher patience = better price)
    float patienceMultiplier = 1.0f + (customer.patience - 1) * 0.1f;
    
    return static_cast<int>(basePrice * patienceMultiplier);
}

/**
 * Check if player has enough items to sell
 * What it does: Verifies whether the player's inventory contains at least the required quantity
 *               of the specified item by calling Player::getItemCount() and comparing with required quantity.
 * Inputs:
 *   - itemName: Name of the item to check for (must match item names in player inventory)
 *   - quantity: Minimum quantity required for the trade
 * Outputs: Returns true if player has at least the required quantity of the item.
 *          Returns false if player has insufficient items or player pointer is null
 */
bool NightPhase::playerHasItem(const std::string& itemName, int quantity) const {
    if (!m_player) return false;
    // Check if player has enough items
    std::string name = itemName;
    int count = m_player->getItemCount(name);
    return count >= quantity;
}

/**
 * Display night phase menu
 * What it does: Prints the main menu options for the night phase to the console, including:
 *               [1] View Customers, [2] Trade with Customer, [3] Black Market, [4] View Inventory,
 *               [5] End Night, [6] Save & Exit
 * Inputs: None
 * Outputs: None (prints formatted menu to standard output)
 */
void NightPhase::displayMenu() const {
    std::cout << "\n=== Night Menu ===" << std::endl;
    std::cout << "[1] View Customers" << std::endl;
    std::cout << "[2] Trade with Customer" << std::endl;
    std::cout << "[3] Black Market" << std::endl;
    std::cout << "[4] View Inventory" << std::endl;
    std::cout << "[5] End Night" << std::endl;
    std::cout << "[6] Save & Exit" << std::endl;
}

/**
 * Handle black market access
 * What it does: Manages the black market interaction loop. Displays black market menu with available
 *               items, processes player input for item selection, prompts for purchase confirmation,
 *               and calls Market::purchaseFromBlackMarket() to complete transactions. Continues looping
 *               until player selects option [0] to return to the main night menu.
 * Inputs: None
 * Outputs: None (modifies player inventory and money through Market::purchaseFromBlackMarket() calls.
 *          Prints black market menu and transaction results to console)
 */
void NightPhase::handleBlackMarket() {
    if (!m_market) {
        std::cout << "Error: Market not initialized" << std::endl;
        return;
    }
    
    bool inBlackMarket = true;
    
    while (inBlackMarket) {
        m_market->displayBlackMarket();
        
        int choice;
        std::cout << "\nChoice: ";
        std::cin >> choice;
        
        if (choice == 0) {
            inBlackMarket = false;
            continue;
        }
        
        const std::vector<BlackMarketItem>& stock = m_market->getBlackMarketStock();
        if (choice > 0 && choice <= static_cast<int>(stock.size())) {
            const BlackMarketItem& item = stock[choice - 1];
            
            std::cout << "\nBuy " << item.display_name << "?" << std::endl;
            std::cout << "Price: " << item.price << " Money" << std::endl;
            std::cout << "[1] Yes [2] No: ";
            
            int confirm;
            std::cin >> confirm;
            
            if (confirm == 1) {
                if (m_player) {
                    m_market->purchaseFromBlackMarket(*m_player, item.item_name);
                }
            }
        } else {
            std::cout << "Invalid choice" << std::endl;
        }
    }
}

/**
 * Process end of night phase
 * What it does: Handles all end-of-night operations. Updates market trends (increases demand values
 *               by growth rate), displays updated demand values to console, charges the daily fee from
 *               player's money, checks if player has sufficient funds to continue, and implements
 *               game over condition if funds are insufficient. This is called when player selects
 *               "End Night" option.
 * Inputs:
 *   - day: Current day number (used for display purposes, though not currently shown in output)
 * Outputs: Returns true if player can continue to next day (sufficient funds for daily fee).
 *          Returns false if game over (insufficient funds to pay daily fee).
 *          Modifies market trends (calls Market::updateTrends()) and player money (deducts DAILY_FEE).
 *          Prints end-of-night information to console.
 */
bool NightPhase::processEndOfNight(int day) {
    if (!m_market) {
        return false;
    }
    
    std::cout << "\n=== End of Night ===" << std::endl;
    
    // Update market trends
    m_market->updateTrends();
    MarketTrends& trends = m_market->getTrends();
    
    std::cout << "Market demand updated:" << std::endl;
    std::cout << "  Food demand: " << std::fixed << std::setprecision(2) 
              << trends.food_demand << std::endl;
    std::cout << "  Medicine demand: " << trends.medicine_demand << std::endl;
    std::cout << "  Metal demand: " << trends.metal_demand << std::endl;
    
    // Charge daily fee
    #ifndef DAILY_FEE
    #define DAILY_FEE 10  // Default daily fee if not defined in Config.h
    #endif
    std::cout << "\nDaily fee required: " << DAILY_FEE << " Money" << std::endl;
    
    // Check if player has enough money
    int playerMoney = m_player->money;
    if (playerMoney < DAILY_FEE) {
        return false;  // Game over
    }
    
    // Deduct fee
    m_player->money -= DAILY_FEE;
    std::cout << "Fee paid. Remaining money: " << m_player->money << std::endl;
    
    std::cout << "Preparing for next day..." << std::endl;
    return true;
}

