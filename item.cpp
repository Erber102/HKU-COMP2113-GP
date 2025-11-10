#include "Item.h"
#include <iostream>
using namespace std;

// display item information
void showItemInfo(Item item) {
    cout << "=== " << item.name << " ===" << endl;
    cout << "Description: " << item.description << endl;
    cout << "Value: " << item.value << endl;
    
    switch (item.category) {
        case CATEGORY_SCRAP:
            cout << "Category: Scrap" << endl;
            break;
        case CATEGORY_FOOD:
            cout << "Category: Food";
            if (item.isEdible) {
                cout << " - Health Recovery:" << item.healthEffect << " Stamina Recovery:" << item.staminaEffect;
            }
            cout << endl;
            break;
        case CATEGORY_MEDICINE:
            cout << "Category: Medical Supplies - Health Recovery:" << item.healthEffect << endl;
            break;
        case CATEGORY_TOOL:
            cout << "Category: Tool - Durability:" << item.durability << endl;
            break;
        case CATEGORY_WEAPON:
            cout << "Category: Weapon - Damage:" << item.damage << " Durability:" << item.durability << endl;
            break;
        case CATEGORY_AMMO:
            cout << "Category: Ammunition" << endl;
            break;
    }
    
    if (item.isEdible) {
        cout << "Status: Edible" << endl;
    }
    cout << endl;
}
