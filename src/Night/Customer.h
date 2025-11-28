#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include <vector>
#include "Market.h"  // For MarketTrends definition

// Forward declarations
struct Item;

/**
 * Customer structure representing a NPC customer
 * Each customer wants to buy specific items with certain quantities
 */
struct Customer {
    std::string name;              // Customer name 
    std::string desired_item;      // Item they want to buy 
    int desired_quantity;          // Quantity they want to buy
    int patience;                  // How long they can tolerate your price
    int base_price;                // Base price they're willing to pay per item
    
    /**
     * Constructor for Customer struct
     * What it does: Initializes a Customer object with specified attributes
     * Inputs:
     *   - n: Customer's name
     *   - item: Name of the item the customer wants to buy
     *   - qty: Quantity of items the customer wants
     *   - pat: Customer's patience level (affects price negotiation)
     *   - price: Base price per item the customer is willing to pay
     * Outputs: None (constructor)
     */
    Customer(const std::string& n, const std::string& item, int qty, int pat, int price);
    
    /**
     * Default constructor for Customer struct
     * What it does: Creates an empty Customer object with default values
     * Inputs: None
     * Outputs: None (constructor)
     */
    Customer();
};

/**
 * Generate random customers for the night phase
 * What it does: Creates a vector of randomly generated customers based on current market trends.
 *               The quantity each customer wants is influenced by the demand growth system,
 *               where later days will have customers requesting more items.
 * Inputs:
 *   - trends: Reference to MarketTrends object containing current demand values for different item types
 *   - count: Number of customers to generate (1-3). If -1, randomly generates between MIN_CUSTOMERS and MAX_CUSTOMERS
 * Outputs: Returns a vector of Customer objects with randomly assigned names, desired items,
 *          quantities (affected by demand trends), patience levels, and base prices
 */
std::vector<Customer> generateCustomers(MarketTrends& trends, int count = -1);

/**
 * Get a random customer name from a predefined list
 * What it does: Selects and returns a random customer name from a predefined pool of names
 *               representing different survivor archetypes (e.g., "Old Hunter", "Anxious Mother")
 * Inputs: None
 * Outputs: Returns a string containing a randomly selected customer name
 */
std::string getRandomCustomerName();

/**
 * Get a random desired item for a customer
 * What it does: Selects and returns a random item name from a predefined list of items
 *               that customers might want to purchase (e.g., "Canned food", "Scrap Metal")
 * Inputs: None
 * Outputs: Returns a string containing a randomly selected item name
 */
std::string getRandomDesiredItem();

#endif // CUSTOMER_H


