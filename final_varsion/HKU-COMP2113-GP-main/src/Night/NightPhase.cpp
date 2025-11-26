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

// Constructor
NightPhase::NightPhase(Player* player, Market* market) 
    : m_player(player), m_market(market) {
}

// Default constructor
NightPhase::NightPhase() 
    : m_player(nullptr), m_market(nullptr) {
}

// Static executeNight method - Note: This signature needs to match Game.cpp usage
// Game.cpp currently passes nullptr for market, so we need to handle that
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

// Execute night phase
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

// Display customers
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

// Handle customer trade
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

// Calculate trade price
int NightPhase::calculateTradePrice(const Customer& customer, int quantity) const {
    // Base price calculation
    int basePrice = customer.base_price * quantity;
    
    // Adjust based on patience (higher patience = better price)
    float patienceMultiplier = 1.0f + (customer.patience - 1) * 0.1f;
    
    return static_cast<int>(basePrice * patienceMultiplier);
}

// Check if player has item
bool NightPhase::playerHasItem(const std::string& itemName, int quantity) const {
    if (!m_player) return false;
    // Check if player has enough items
    std::string name = itemName;
    int count = m_player->getItemCount(name);
    return count >= quantity;
}

// Display menu
void NightPhase::displayMenu() const {
    std::cout << "\n=== Night Menu ===" << std::endl;
    std::cout << "[1] View Customers" << std::endl;
    std::cout << "[2] Trade with Customer" << std::endl;
    std::cout << "[3] Black Market" << std::endl;
    std::cout << "[4] View Inventory" << std::endl;
    std::cout << "[5] End Night" << std::endl;
    std::cout << "[6] Save & Exit" << std::endl;
}

// Handle black market
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

// Process end of night
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

