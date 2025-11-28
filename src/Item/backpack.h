#ifndef BACKPACK_H
#define BACKPACK_H

#include <vector>
#include "item.h"

class Backpack {
private:
    std::vector<Item> items;   // internal storage of items

public:
    bool addItem(const Item& item);
    void removeItem(int index);
    Item getItem(int index) const;
    void displayItems() const;
    bool isFull() const;
};

#endif
