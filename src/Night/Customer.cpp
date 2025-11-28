#include "Customer.h"
#include "Market.h"  // Include here for MarketTrends definition
#include "../Core/Config.h"
#include "../Item/item.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iostream>

// Customer generation related constants (use these defaults if not defined in Config.h)
#ifndef MIN_CUSTOMERS
#define MIN_CUSTOMERS 1
#endif
#ifndef MAX_CUSTOMERS
#define MAX_CUSTOMERS 3
#endif
#ifndef MIN_PATIENCE
#define MIN_PATIENCE 1
#endif
#ifndef MAX_PATIENCE
#define MAX_PATIENCE 5
#endif
#ifndef MIN_PRICE_MULTIPLIER
#define MIN_PRICE_MULTIPLIER 0.8f
#endif
#ifndef MAX_PRICE_MULTIPLIER
#define MAX_PRICE_MULTIPLIER 1.2f
#endif

/**
 * Customer constructor
 * What it does: Initializes a Customer object with the provided name, desired item, quantity, patience, and base price
 * Inputs:
 *   - n: Customer's name string
 *   - item: Name of the item the customer wants to buy
 *   - qty: Quantity of items desired
 *   - pat: Patience level (affects price negotiation)
 *   - price: Base price per item
 * Outputs: None (constructor initializes member variables)
 */
Customer::Customer(const std::string& n, const std::string& item, int qty, int pat, int price)
    : name(n), desired_item(item), desired_quantity(qty), patience(pat), base_price(price) {
}

/**
 * Default constructor for Customer
 * What it does: Creates an empty Customer object with all fields initialized to empty strings or zero
 * Inputs: None
 * Outputs: None (constructor initializes all member variables to default values)
 */
Customer::Customer() 
    : name(""), desired_item(""), desired_quantity(0), patience(0), base_price(0) {
}

/**
 * Get random customer name
 * What it does: Returns a randomly selected customer name from a predefined list of survivor archetypes.
 *               Initializes random seed on first call if not already initialized.
 * Inputs: None
 * Outputs: Returns a string containing a randomly selected customer name from the predefined list
 */
std::string getRandomCustomerName() {
    static const std::vector<std::string> customerNames = {
        "Old Hunter", "Anxious Mother", "Wounded Soldier", "Cunning Merchant",
        "Desperate Survivor", "Young Scavenger", "Experienced Explorer", "Mysterious Stranger",
        "Hungry Wanderer", "Medicine Seeker"
    };
    
    static bool initialized = false;
    if (!initialized) {
        srand(static_cast<unsigned int>(time(NULL)));
        initialized = true;
    }
    
    int index = rand() % customerNames.size();
    return customerNames[index];
}

/**
 * Get random desired item
 * What it does: Returns a randomly selected item name from a predefined list of items that customers might want.
 *               Initializes random seed on first call if not already initialized.
 * Inputs: None
 * Outputs: Returns a string containing a randomly selected item name from the predefined list
 */
std::string getRandomDesiredItem() {
    static const std::vector<std::string> desiredItems = {
        "Canned food", "Scrap Metal", "Electronic Component", "Gold Ring",
        "Rag", "Bandage", "First Aid Kit", "Military Rations"
    };
    
    static bool initialized = false;
    if (!initialized) {
        srand(static_cast<unsigned int>(time(NULL)));
        initialized = true;
    }
    
    int index = rand() % desiredItems.size();
    return desiredItems[index];
}

/**
 * Generate random customers
 * What it does: Creates a vector of randomly generated customers for the night phase.
 *               Each customer is assigned a random name, desired item, and patience level.
 *               The desired quantity is calculated using the demand growth system: base quantity
 *               plus a random factor multiplied by the current market demand for that item type.
 *               This implements the core innovation where customer demands grow over time.
 * Inputs:
 *   - trends: Reference to MarketTrends object containing current demand multipliers for different item categories
 *   - count: Number of customers to generate. If negative, randomly generates between MIN_CUSTOMERS and MAX_CUSTOMERS
 * Outputs: Returns a vector of Customer objects, each with randomly assigned attributes and
 *          demand-influenced quantities
 */
std::vector<Customer> generateCustomers(MarketTrends& trends, int count) {
    // Initialize random seed if not already done
    static bool initialized = false;
    if (!initialized) {
        srand(static_cast<unsigned int>(time(NULL)));
        initialized = true;
    }
    
    // Determine number of customers (1-3)
    if (count < 0) {
        count = MIN_CUSTOMERS + (rand() % (MAX_CUSTOMERS - MIN_CUSTOMERS + 1));
    }
    
    std::vector<Customer> customers;
    customers.reserve(count);
    
    for (int i = 0; i < count; i++) {
        std::string name = getRandomCustomerName();
        std::string item = getRandomDesiredItem();
        
        // Base quantity (1-3 items)
        int baseQuantity = 1 + (rand() % 3);
        
        // Get demand multiplier for this item type
        float demandMultiplier = trends.getDemandForItem(item);
        
        // Calculate desired quantity with demand growth
        // desired_quantity = base_amount + (random_factor * MarketTrends.demand)
        int randomFactor = 1 + (rand() % 3);  // Random factor 1-3
        int desiredQuantity = baseQuantity + static_cast<int>(randomFactor * demandMultiplier);
        
        // Ensure minimum of 1
        if (desiredQuantity < 1) desiredQuantity = 1;
        
        // Generate patience (1-5)
        int patience = MIN_PATIENCE + (rand() % (MAX_PATIENCE - MIN_PATIENCE + 1));
        
        // Generate base price (based on item value from database)
        // We'll use a default value and adjust based on item type
        int basePrice = 5;  // Default base price
        if (item == "Canned food" || item == "Military Rations") {
            basePrice = 3;
        } else if (item == "Gold Ring" || item == "Electronic Component") {
            basePrice = 10;
        } else if (item == "Scrap Metal") {
            basePrice = 2;
        } else if (item == "First Aid Kit" || item == "Bandage") {
            basePrice = 8;
        }
        
        // Apply price multiplier based on patience
        float priceMultiplier = MIN_PRICE_MULTIPLIER + 
            (static_cast<float>(rand()) / RAND_MAX) * (MAX_PRICE_MULTIPLIER - MIN_PRICE_MULTIPLIER);
        basePrice = static_cast<int>(basePrice * priceMultiplier);
        
        customers.push_back(Customer(name, item, desiredQuantity, patience, basePrice));
    }
    
    return customers;
}

