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
    
    // Constructor
    Customer(const std::string& n, const std::string& item, int qty, int pat, int price);
    Customer();  // Default constructor
};

/**
 * Generate random customers for the night phase
 * @param trends Reference to MarketTrends to calculate demand-based quantities
 * @param count Number of customers to generate (1-3)
 * @return Vector of generated customers
 */
std::vector<Customer> generateCustomers(MarketTrends& trends, int count = -1);

/**
 * Get a random customer name from a predefined list
 * @return Random customer name
 */
std::string getRandomCustomerName();

/**
 * Get a random desired item for a customer
 * @return Random item name
 */
std::string getRandomDesiredItem();

#endif // CUSTOMER_H


