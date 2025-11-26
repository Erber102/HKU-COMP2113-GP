#include "itemdatabase.h"
#include "../Core/Config.h"
#include <cstdlib>
#include <ctime>
#include <string>
using namespace std;

Item itemPrototypes[20];  // To store the definition of items
int itemPrototypeCount = 0;
int currentDifficulty = DIFFICULTY_NORMAL;  // current difficulty

// function that creates items' prototype
void createItemPrototype(int id, string name, string desc, int cat, int val, int health, int stamina, int dmg, int dur, bool edible) {
    if (itemPrototypeCount >= 20){
      return;
    }
    Item newItem;
    newItem.id = id;
    newItem.name = name;
    newItem.description = desc;
    newItem.category = cat;
    newItem.value = val;
    newItem.healthEffect = health;
    newItem.staminaEffect = stamina;
    newItem.damage = dmg;
    newItem.durability = dur;
    newItem.isEdible = edible;
    
    itemPrototypes[itemPrototypeCount] = newItem;
    itemPrototypeCount++;
}

// function that initializes the data base
void initItemDatabase() {
    itemPrototypeCount = 0;
    
    // === Scrap ===
    // Low-value scrap (2 items)
    createItemPrototype(0, "Scrap Metal", "Rusty metal fragments", CATEGORY_SCRAP, 3, 0, 0, 0, 0, false);
    createItemPrototype(1, "Rag", "Dirty piece of cloth", CATEGORY_SCRAP, 2, 0, 0, 0, 0, false);
    
    // Medium-value scrap (1 item)
    createItemPrototype(2, "Electronic Component", "Damaged circuit component", CATEGORY_SCRAP, 8, 0, 0, 0, 0, false);
    
    // High-value scrap (1 item)
    createItemPrototype(3, "Gold Ring", "Shiny gold ring", CATEGORY_SCRAP, 15, 0, 0, 0, 0, false);
    
    // Canned food (both scrap and food)
    createItemPrototype(4, "Canned food", "Edible canned food", CATEGORY_FOOD, 2, 20, 10, 0, 0, true);
    
    // === Goods sold in the store ===
    // Food
    createItemPrototype(5, "Military Rations", "High-quality food", CATEGORY_FOOD, 25, 40, 25, 0, 0, true);
    
    // Medical
    createItemPrototype(6, "Bandage", "Basic medical supplies", CATEGORY_MEDICINE, 5, 15, 0, 0, 0, false);
    createItemPrototype(7, "First Aid Kit", "Advanced medical supplies", CATEGORY_MEDICINE, 30, 50, 0, 0, 0, false);
    
    // Tools
    createItemPrototype(8, "Crowbar", "Open sealed containers", CATEGORY_TOOL, 25, 0, 0, 0, 20, false);
    createItemPrototype(9, "Lock pick", "Professional lock-picking tool", CATEGORY_TOOL, 45, 0, 0, 0, 15, false);
    
    // Weapons
    createItemPrototype(10, "Baseball Bat", "Reliable melee weapon", CATEGORY_WEAPON, 20, 0, 0, 15, 50, false);
    createItemPrototype(11, "Shotgun", "Reliable ranged weapon", CATEGORY_WEAPON, 80, 0, 0, 45, 30, false);
    
    // Ammunition
    createItemPrototype(12, "Bullet", "Shotgun ammunition", CATEGORY_AMMO, 2, 0, 0, 0, 0, false);
    
    // Initialize seed for rand()
    srand(time(NULL));
}

// function that sets the difficulty without generating error
void setGameDifficulty(int difficulty) {
    if (difficulty >= DIFFICULTY_EASY && difficulty <= DIFFICULTY_HARD) {
        currentDifficulty = difficulty;
    }
}

// function that gets the current difficulty
int getGameDifficulty() {
    return currentDifficulty;
}


// function that creates item according to the name
std::unique_ptr<Item> createItem(string itemName) {
    for (int i = 0; i < itemPrototypeCount; i++) {
        if (itemPrototypes[i].name == itemName) {
            // Use unique_ptr with new to create new instance
            std::unique_ptr<Item> newItem(new Item());
            *newItem = itemPrototypes[i];
            return newItem;
        }
    }
    return nullptr;
}

// function that gives random scrap item according to difficulty
std::unique_ptr<Item> getRandomScrapWithDifficulty(int difficulty) {
    // Collect all scrap items (including canned food)
    int scrapItems[20];
    int scrapCount = 0;
    
    for (int i = 0; i < itemPrototypeCount; i++) {
        if (itemPrototypes[i].category == CATEGORY_SCRAP || itemPrototypes[i].id == 4) { // Canned food ID is 4
            scrapItems[scrapCount] = i;
            scrapCount++;
        }
    }
    
    if (scrapCount == 0) {
        return nullptr;
    }
    
    // categorize scrap by value
    int lowValueItems[20];    // Low-value scrap (value 1-3)
    int mediumValueItems[20]; // Medium-value scrap (value 4-10)
    int highValueItems[20];   // High-value scrap (value 11+)
    int lowCount = 0, mediumCount = 0, highCount = 0;
    
    for (int i = 0; i < scrapCount; i++) {
        int index = scrapItems[i];
        int value = itemPrototypes[index].value;
        
        if (value <= 3) {
            lowValueItems[lowCount] = index;
            lowCount++;
        } 
	else if (value <= 10) {
            mediumValueItems[mediumCount] = index;
            mediumCount++;
        } 
	else {
            highValueItems[highCount] = index;
            highCount++;
        }
    }
    
    // set probability distribution based on difficulty
    int randomValue = rand() % 100;  // Random number 0-99

    switch (difficulty) {
        case DIFFICULTY_EASY:
            // Easy: Low 40%, Medium 35%, High 25%
            if (randomValue < 25) {
                // Only 1 high-value item, so we just use index 0
                std::unique_ptr<Item> newItem(new Item());
                *newItem = itemPrototypes[highValueItems[0]];  // Gold Ring
                return newItem;
            }
            else if (randomValue < 60) {
                // Only 1 medium-value item, so we just use index 0
                std::unique_ptr<Item> newItem(new Item());
                *newItem = itemPrototypes[mediumValueItems[0]];  // Electronic Component
                return newItem;
            }
            else if (lowCount > 0) {
                // Multiple low-value items, so we need random selection
                int selectedIndex = rand() % lowCount;
                std::unique_ptr<Item> newItem(new Item());
                *newItem = itemPrototypes[lowValueItems[selectedIndex]];  // Random low-value
                return newItem;
            }
            break;

        case DIFFICULTY_NORMAL:
            // Normal: Low 60%, Medium 30%, High 10%
            if (randomValue < 10) {
                std::unique_ptr<Item> newItem(new Item());
                *newItem = itemPrototypes[highValueItems[0]];  // Gold Ring
                return newItem;
            }
            else if (randomValue < 40) {
                std::unique_ptr<Item> newItem(new Item());
                *newItem = itemPrototypes[mediumValueItems[0]];  // Electronic Component
                return newItem;
            }
            else if (lowCount > 0) {
                int selectedIndex = rand() % lowCount;
                std::unique_ptr<Item> newItem(new Item());
                *newItem = itemPrototypes[lowValueItems[selectedIndex]];  // Random low-value
                return newItem;
            }
            break;

        case DIFFICULTY_HARD:
            // Hard: Low 80%, Medium 15%, High 5%
            if (randomValue < 5) {
                std::unique_ptr<Item> newItem(new Item());
                *newItem = itemPrototypes[highValueItems[0]];  // Gold Ring
                return newItem;
            }
            else if (randomValue < 20) {
                std::unique_ptr<Item> newItem(new Item());
                *newItem = itemPrototypes[mediumValueItems[0]];  // Electronic Component
                return newItem;
            }
            else if (lowCount > 0) {
                int selectedIndex = rand() % lowCount;
                std::unique_ptr<Item> newItem(new Item());
                *newItem = itemPrototypes[lowValueItems[selectedIndex]];  // Random low-value
                return newItem;
            }
            break;
    }

    // If all conditions fail, return random scrap item
    int randomIndex = rand() % scrapCount;
    std::unique_ptr<Item> newItem(new Item());
    *newItem = itemPrototypes[scrapItems[randomIndex]];
    return newItem;
}

// Get shop item by index
std::unique_ptr<Item> getShopItem(int index) {
    if (index < 0 || index >= getShopItemCount()) {
        cout << "Error: Invalid shop item index!" << endl;
        return nullptr;
    }
    
    int count = 0;
    for (int i = 0; i < itemPrototypeCount; i++) {
        if (itemPrototypes[i].category != CATEGORY_SCRAP) {
            if (count == index) {
                std::unique_ptr<Item> newItem(new Item());
                *newItem = itemPrototypes[i];
                return newItem;
            }
            count++;
        }
    }

    return nullptr;
}

// Get shop item count
int getShopItemCount() {
    int count = 0;
    for (int i = 0; i < itemPrototypeCount; i++) {
        if (itemPrototypes[i].category != CATEGORY_SCRAP) {
            count++;
        }
    }
    return count;
}

// Gets the most expensive item from the database
std::unique_ptr<Item> getMostExpensiveItem() {
    if (itemPrototypeCount == 0) {
        cout << "Error: No items in database!" << endl;
        return nullptr;
    }
    
    int maxIndex = 0;
    int maxValue = itemPrototypes[0].value;
    
    for (int i = 1; i < itemPrototypeCount; i++) {
        if (itemPrototypes[i].value > maxValue) {
            maxValue = itemPrototypes[i].value;
            maxIndex = i;
        }
    }
    
    std::unique_ptr<Item> expensiveItem(new Item());
    *expensiveItem = itemPrototypes[maxIndex];
    return expensiveItem;
}

// Gets the cheapest item from the database
std::unique_ptr<Item> getCheapestItem() {
    if (itemPrototypeCount == 0) {
        cout << "Error: No items in database!" << endl;
        return nullptr;
    }
    
    int minIndex = 0;
    int minValue = itemPrototypes[0].value;
    
    for (int i = 1; i < itemPrototypeCount; i++) {
        if (itemPrototypes[i].value < minValue) {
            minValue = itemPrototypes[i].value;
            minIndex = i;
        }
    }
    
    std::unique_ptr<Item> cheapItem(new Item());
    *cheapItem = itemPrototypes[minIndex];
    return cheapItem;
}

// Finds item by ID
std::unique_ptr<Item> findItemById(int id) {
    if (id < 0) {
        cout << "Error: Invalid item ID!" << endl;
        return nullptr;
    }
    
    for (int i = 0; i < itemPrototypeCount; i++) {
        if (itemPrototypes[i].id == id) {
            std::unique_ptr<Item> foundItem(new Item());
            *foundItem = itemPrototypes[i];
            return foundItem;
        }
    }

    cout << "Error: Item with ID " << id << " not found!" << endl;
    return nullptr;
}

// Gets total number of item prototypes
int getTotalItemPrototypes() {
    return itemPrototypeCount;
}

// Validates item data
bool isValidItem(Item* item) {
    if (item == NULL) {
        cout << "Error: Item pointer is NULL!" << endl;
        return false;
    }
    
    if (item->id < 0) {
        cout << "Error: Item ID cannot be negative!" << endl;
        return false;
    }
    
    if (item->name.empty()) {
        cout << "Error: Item name cannot be empty!" << endl;
        return false;
    }
    
    if (item->value < 0) {
        cout << "Error: Item value cannot be negative!" << endl;
        return false;
    }
    
    if (item->category < CATEGORY_SCRAP || item->category > CATEGORY_AMMO) {
        cout << "Error: Invalid item category!" << endl;
        return false;
    }
    
    return true;
}
