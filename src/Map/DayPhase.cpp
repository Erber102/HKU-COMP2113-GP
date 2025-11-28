#include "DayPhase.h"
#include "../Player/Player.h"
#include "../Item/itemdatabase.h"
#include "../Core/UISystem.h"
#include "../Combat/combat.h"
#include "../Save/SaveSystem.h"
#include <unordered_map>
#include <iostream>
#include <limits>
#include <cstdlib>

// Constructor: Initializes DayPhase with player, map, and item database
// What it does: Sets up the DayPhase object with references to game components
// Inputs: player - Pointer to Player object, map - Pointer to Map object, itemDB - Pointer to ItemDatabase
// Outputs: None (constructor)
DayPhase::DayPhase(Player* player, Map* map, ItemDatabase* itemDB)
    : m_player(player), m_map(map), m_itemDB(itemDB), m_currentDay(1) {}

namespace {
// Helper function: Translates internal loot names to display names
// What it does: Converts internal item IDs to user-friendly display names using a mapping table
// Inputs: raw - Internal item ID string
// Outputs: string - User-friendly display name
std::string translateLootName(const std::string& raw) {
    static const std::unordered_map<std::string, std::string> mapping = {
        {"bandage", "Bandage"},
        // Map logical loot IDs to actual item names in ItemDatabase
        {"water", "Water"},
        {"canned_food", "Canned food"},
        {"pistol", "Baseball Bat"},
        {"medkit", "First Aid Kit"},
        {"rifle", "Shotgun"},
        {"grenade", "Bullet"},
        {"energy_drink", "Military Rations"},
        {"sniper_rifle", "Shotgun"},
        {"armor", "Crowbar"},
        {"lockpick", "Lock pick"},
        {"crowbar", "Crowbar"},
        {"explosives", "Bullet"},
        {"master_key", "Lock pick"}
    };
    auto it = mapping.find(raw);
    return it != mapping.end() ? it->second : raw;
}
}

// Main day execution loop: Handles the day phase exploration
// What it does: Manages the day exploration phase, allowing player to select and explore locations
// Inputs: currentDay - Current day number for tracking progress
// Outputs: None
void DayPhase::executeDay(int currentDay) {
    m_currentDay = currentDay;
    UISystem::startSection("DAY PHASE");
    UISystem::showInfo("Exploring locations...");

    while (true) {
        Location* selectedLocation = selectLocationMenu();
        if (!selectedLocation) {
            UISystem::showInfo("You decided to return to camp and rest...");
            break;
        }

        exploreLocation(selectedLocation);

        UISystem::showInfo("Continue exploring? (y/n) or press S to save & quit");
        bool continueExploring = true;
        bool waiting = true;
        while (waiting) {
            if (InputSystem::kbhit()) {
                char choice = InputSystem::getch();
                if (choice == 's' || choice == 'S') {
                    performSaveAndExit();
                }
                if (choice == 'y' || choice == 'Y') {
                    waiting = false;
                } else if (choice == 'n' || choice == 'N') {
                    continueExploring = false;
                    waiting = false;
                }
            }
            InputSystem::sleepMs(100);
        }
        if (!continueExploring) {
            UISystem::showInfo("You decided to return to camp...");
            break;
        }
    }

    UISystem::endSection();
}

// Location selection menu: Displays available locations for exploration
// What it does: Shows a menu of unexplored locations and handles user selection
// Inputs: None
// Outputs: Location* - Pointer to selected location, or nullptr if user cancels
Location* DayPhase::selectLocationMenu() {
    auto availableLocations = m_map->getAvailableLocations();

    if (availableLocations.empty()) {
        std::cout << "All locations have been explored!" << std::endl;
        return nullptr;
    }

    while (true) {
        std::cout << "=== Select Exploration Location ===" << std::endl;
        for (size_t i = 0; i < availableLocations.size(); ++i) {
            std::cout << i + 1 << ". " << availableLocations[i]->name
                      << " (Danger Level: " << availableLocations[i]->dangerLevel << ")" << std::endl;
        }
        std::cout << "0. Return to Camp" << std::endl;
        std::cout << "[S] Save & Quit" << std::endl;
        std::cout << "Please choose (0-" << availableLocations.size() << "): ";

        bool awaitingInput = true;
        while (awaitingInput) {
            if (InputSystem::kbhit()) {
                char key = InputSystem::getch();
                if (key == 's' || key == 'S') {
                    performSaveAndExit();
                }
                if (key >= '0' && key <= '9') {
                    int choice = key - '0';
                    if (choice == 0) {
                        return nullptr;
                    }
                    if (choice >= 1 && choice <= static_cast<int>(availableLocations.size())) {
                        return availableLocations[choice - 1];
                    }
                    std::cout << "Invalid choice!" << std::endl;
                    awaitingInput = false;
                } else {
                    std::cout << "Invalid input!" << std::endl;
                    awaitingInput = false;
                }
            }
            InputSystem::sleepMs(100);
        }
    }
}

// Location exploration: Handles the exploration of a selected location
// What it does: Generates and processes events for the selected location, marks it as completed
// Inputs: location - Pointer to the location to explore
// Outputs: None
void DayPhase::exploreLocation(Location* location) {
    std::cout << "\n=== Exploring " << location->name << " ===" << std::endl;
    std::cout << "Danger Level: " << location->dangerLevel << std::endl;
    std::cout << "Searching..." << std::endl;

    // Generate random event
    EventResult result = EventFactory::generateEvent(*location);

    // Handle event result
    handleEventResult(result);

    // Mark location as explored
    m_map->completeLocation(location);
}

// Event result handler: Processes different types of events and their outcomes
// What it does: Handles loot collection, enemy encounters, special discoveries, and empty results
// Inputs: result - EventResult object containing event type and data
// Outputs: None
void DayPhase::handleEventResult(const EventResult& result) {
    std::cout << "\n" << result.message << std::endl;

    switch (result.type) {
        case EventType::LOOT_FOUND:
            std::cout << "Found supplies:" << std::endl;
            for (const auto& item : result.loot) {
                std::cout << "  - " << item.first << " x" << item.second << std::endl;
                std::string mappedName = translateLootName(item.first);
                for (int i = 0; i < item.second; ++i) {
                    Item* newItem = createItem(mappedName);
                    if (newItem) {
                        m_player->addItem(newItem);
                    } else {
                        std::cout << "    Unable to store " << mappedName << std::endl;
                    }
                }
            }
            break;

        case EventType::ENEMY_ENCOUNTER: {
            std::cout << "Encountered enemy: " << result.enemyId << "! Prepare for combat!" << std::endl;
            int enemyIndex = 0;
            if (result.enemyId == "mutant_dog") {
                enemyIndex = 1;
            } else if (result.enemyId == "raider") {
                enemyIndex = 2;
            } else if (result.enemyId == "mutant_beast") {
                enemyIndex = 3;
            } else if (result.enemyId == "boss") {
                enemyIndex = 3;
            }
            Item* bestWeapon = m_player->getBestWeapon();
            Combat combat(m_player, "Survivor", bestWeapon);
            combat.fightEnemy(enemyIndex);
            break;
        }

        case EventType::SPECIAL_DISCOVERY:
            if (result.requiredItem == "none") {
                std::cout << "Special discovery:" << std::endl;
                for (const auto& item : result.loot) {
                    std::cout << "  - " << item.first << " x" << item.second << std::endl;
                    std::string mappedName = translateLootName(item.first);
                    for (int i = 0; i < item.second; ++i) {
                        Item* newItem = createItem(mappedName);
                        if (newItem) {
                            m_player->addItem(newItem);
                        }
                    }
                }
            } else {
                std::string requiredItem = translateLootName(result.requiredItem);
                std::cout << "You need " << requiredItem << " to open this container." << std::endl;
                if (m_player->hasItem(requiredItem)) {
                    std::cout << "You used " << requiredItem << " to open it!" << std::endl;
                    m_player->removeItem(requiredItem);
                    for (const auto& item : result.loot) {
                        std::cout << "  - " << item.first << " x" << item.second << std::endl;
                        std::string mappedName = translateLootName(item.first);
                        for (int i = 0; i < item.second; ++i) {
                            Item* newItem = createItem(mappedName);
                            if (newItem) {
                                m_player->addItem(newItem);
                            }
                        }
                    }
                } else {
                    std::cout << "You don't have the required tool, have to give up..." << std::endl;
                }
            }
            break;

        case EventType::NOTHING_FOUND:
            std::cout << "This search yielded nothing..." << std::endl;
            break;
    }
}

// Save and exit function: Saves game progress and exits the game
// What it does: Calls the save system to persist player data and current day, then terminates the program
// Inputs: None
// Outputs: None (terminates program)
void DayPhase::performSaveAndExit() const {
    SaveSystem::saveGame(*m_player, m_currentDay);
    std::cout << "Progress saved. Exiting game..." << std::endl;
    std::exit(0);
}