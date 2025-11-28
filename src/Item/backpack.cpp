
#include "backpack.h"
#include <iostream>

bool Backpack::addItem(const Item& item) {
    
    if (isFull()) {
        cout << "Backpack is full, cannot add item!" << endl;
        return false;
    }
    items.push_back(item);
    return true;
}

void Backpack::removeItem(int index) {
    if (index >= 0 && static_cast<size_t>(index) < items.size()) {
        items.erase(items.begin() + index);
    }
}

Item Backpack::getItem(int index) const {
    if (index >= 0 && static_cast<size_t>(index) < items.size()) {
        return items[index];
    }
    // Return a default item to indicate error
    return Item{0, "Invalid Item", "This item does not exist", 0, 0, 0, 0, 0, 0, false};
}

void Backpack::displayItems() const {
    // If there are no items stored in the internal container (e.g. std::vector<Item> items), print a message and exit early
    if (items.empty()) {
        cout << "Backpack is empty" << endl;
        return;
    }
    
    // Print a header so the player knows they are looking at the backpack UI
    cout << "=== Backpack Items ===" << endl;
    // For each item, it prints the item's name and description
    for (size_t i = 0; i < items.size(); i++) {
        cout << i + 1 << ". " << items[i].name << " - " << items[i].description << endl;
    }
    cout << "=================" << endl;
}
