#include "Customer.h"
#include "Market.h"  // Include here for MarketTrends definition
#include "Config.h"
#include "item.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iostream>

// 客户生成相关常量（如果Config.h中没有定义，使用这些默认值）
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

// Customer constructor
Customer::Customer(const std::string& n, const std::string& item, int qty, int pat, int price)
    : name(n), desired_item(item), desired_quantity(qty), patience(pat), base_price(price) {
}

// Default constructor
Customer::Customer() 
    : name(""), desired_item(""), desired_quantity(0), patience(0), base_price(0) {
}

// Get random customer name
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

// Get random desired item
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

// Generate random customers
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

