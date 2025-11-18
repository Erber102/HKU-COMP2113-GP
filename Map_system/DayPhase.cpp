#include "DayPhase.h"
#include "Player.h"
#include "ItemDatabase.h"
#include "CombatSystem.h"
#include <iostream>
#include <limits>

DayPhase::DayPhase(Player* player, Map* map, ItemDatabase* itemDB, CombatSystem* combat)
    : m_player(player), m_map(map), m_itemDB(itemDB), m_combat(combat) {}

void DayPhase::executeDay() {
    std::cout << "=== Day Phase Start ===" << std::endl;
    std::cout << "Stamina: " << m_player->getStamina() << "/" << m_player->getMaxStamina() << std::endl;
    std::cout << std::endl;

    while (m_player->getStamina() > 0) {
        Location* selectedLocation = selectLocationMenu();
        if (!selectedLocation) {
            std::cout << "You decided to return to camp and rest..." << std::endl;
            break;
        }

        exploreLocation(selectedLocation);

        // Consume stamina for each exploration
        m_player->setStamina(m_player->getStamina() - 1);

        std::cout << "Remaining Stamina: " << m_player->getStamina() << std::endl;
        std::cout << "------------------------" << std::endl;

        if (m_player->getStamina() <= 0) {
            std::cout << "Stamina exhausted! Must return to camp to rest." << std::endl;
            break;
        }

        // Ask if player wants to continue exploring
        std::cout << "Continue exploring? (y/n): ";
        char choice;
        std::cin >> choice;
        if (choice != 'y' && choice != 'Y') {
            std::cout << "You decided to return to camp..." << std::endl;
            break;
        }
    }

    std::cout << "=== Day Phase End ===" << std::endl;
}

Location* DayPhase::selectLocationMenu() {
    auto availableLocations = m_map->getAvailableLocations();

    if (availableLocations.empty()) {
        std::cout << "All locations have been explored!" << std::endl;
        return nullptr;
    }

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
        return selectLocationMenu();
    }

    if (choice == 0) {
        return nullptr;
    }

    if (choice < 1 || choice > static_cast<int>(availableLocations.size())) {
        std::cout << "Invalid choice!" << std::endl;
        return selectLocationMenu();
    }

    return availableLocations[choice - 1];
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
                for (int i = 0; i < item.second; ++i) {
                    Item* newItem = m_itemDB->createItem(item.first);
                    if (newItem) {
                        m_player->addItem(newItem);
                    }
                }
            }
            break;

        case EventType::ENEMY_ENCOUNTER:
            std::cout << "Encountered enemy: " << result.enemyId << "! Prepare for combat!" << std::endl;
            // Call combat system
            m_combat->startCombat(m_player, result.enemyId);
            break;

        case EventType::SPECIAL_DISCOVERY:
            if (result.requiredItem == "none") {
                std::cout << "Special discovery:" << std::endl;
                for (const auto& item : result.loot) {
                    std::cout << "  - " << item.first << " x" << item.second << std::endl;
                    for (int i = 0; i < item.second; ++i) {
                        Item* newItem = m_itemDB->createItem(item.first);
                        if (newItem) {
                            m_player->addItem(newItem);
                        }
                    }
                }
            } else {
                std::cout << "You need " << result.requiredItem << " to open this container." << std::endl;
                // Check if player has the required item
                if (m_player->hasItem(result.requiredItem)) {
                    std::cout << "Used " << result.requiredItem << " successfully!" << std::endl;
                    // Additional rewards can be added here
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