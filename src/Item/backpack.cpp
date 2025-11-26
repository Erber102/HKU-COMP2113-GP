// backpack.cpp
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
    if (items.empty()) {
        cout << "Backpack is empty" << endl;
        return;
    }

    cout << "=== Backpack Items ===" << endl;
    for (size_t i = 0; i < items.size(); i++) {
        cout << i + 1 << ". " << items[i].name << " - " << items[i].description << endl;
    }
    cout << "=================" << endl;
}