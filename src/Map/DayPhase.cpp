#include "DayPhase.h"
#include "../Player/Player.h"
#include "../Item/itemdatabase.h"
#include "../Core/UISystem.h"
#include <iostream>
#include <limits>

DayPhase::DayPhase(Player* player, Map* map, ItemDatabase* itemDB)
    : m_player(player), m_map(map), m_itemDB(itemDB) {}

void DayPhase::executeDay() {
    UISystem::startSection("DAY PHASE");
    UISystem::showInfo("Exploring locations...");

    while (true) {
        Location* selectedLocation = selectLocationMenu();
        if (!selectedLocation) {
            UISystem::showInfo("You decided to return to camp and rest...");
            break;
        }

        exploreLocation(selectedLocation);

        UISystem::showInfo("Continue exploring? (y/n)");
        char choice = InputSystem::getch();
        if (choice != 'y' && choice != 'Y') {
            UISystem::showInfo("You decided to return to camp...");
            break;
        }
    }

    UISystem::endSection();
}

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
        std::cout << "Please choose (0-" << availableLocations.size() << "): ";

        int choice;
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input!" << std::endl;
            continue;
        }

        if (choice == 0) {
            return nullptr;
        }

        if (choice < 1 || choice > static_cast<int>(availableLocations.size())) {
            std::cout << "Invalid choice!" << std::endl;
            continue;
        }

        return availableLocations[choice - 1];
    }
}

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

void DayPhase::handleEventResult(const EventResult& result) {
    std::cout << "\n" << result.message << std::endl;

    switch (result.type) {
        case EventType::LOOT_FOUND:
            std::cout << "Found supplies:" << std::endl;
            for (const auto& item : result.loot) {
                std::cout << "  - " << item.first << " x" << item.second << std::endl;
                // Call ItemDatabase to create items and add to player inventory
                // Note: Combat system Player doesn't have addItem method
                // for (int i = 0; i < item.second; ++i) {
                //     Item* newItem = createItem(item.first);
                //     if (newItem) {
                //         m_player->addItem(newItem);
                //     }
                // }
            }
            break;

        case EventType::ENEMY_ENCOUNTER: {
            std::cout << "Encountered enemy: " << result.enemyId << "! Prepare for combat!" << std::endl;
            // Combat system is temporarily disabled
            std::cout << "Combat system not available. You managed to escape!" << std::endl;
            break;
        }

        case EventType::SPECIAL_DISCOVERY:
            if (result.requiredItem == "none") {
                std::cout << "Special discovery:" << std::endl;
                for (const auto& item : result.loot) {
                    std::cout << "  - " << item.first << " x" << item.second << std::endl;
                    // Call ItemDatabase to create items and add to player inventory
                    // Note: Combat system Player doesn't have addItem method
                    // for (int i = 0; i < item.second; ++i) {
                    //     Item* newItem = createItem(item.first);
                    //     if (newItem) {
                    //         m_player->addItem(newItem);
                    //     }
                    // }
                }
            } else {
                std::cout << "You need " << result.requiredItem << " to open this container." << std::endl;
                // Check if player has the required item
                // Note: Combat system Player doesn't have hasItem method
                // if (m_player->hasItem(result.requiredItem)) {
                //     std::cout << "Used " << result.requiredItem << " successfully!" << std::endl;
                //     // Additional rewards can be added here
                // } else {
                //     std::cout << "You don't have the required tool, have to give up..." << std::endl;
                // }
                std::cout << "Item system integration pending..." << std::endl;
            }
            break;

        case EventType::NOTHING_FOUND:
            std::cout << "This search yielded nothing..." << std::endl;
            break;
    }
}