#include "DayPhase.h"
#include "../Player/Player.h"
#include "../Item/itemdatabase.h"
#include "../Core/UISystem.h"
#include "../Combat/combat.h"
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

        int choice = UISystem::getNumericChoice(
            0,
            static_cast<int>(availableLocations.size()),
            "Please choose (0-" + std::to_string(availableLocations.size()) + "):"
        );

        if (choice == -1) {
            return nullptr;
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
                for (int i = 0; i < item.second; ++i) {
                    Item* newItem = createItem(item.first);
                    if (newItem) {
                        m_player->addItem(newItem);
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
            Combat combat("Survivor");
            combat.fightEnemy(enemyIndex);
            break;
        }

        case EventType::SPECIAL_DISCOVERY:
            if (result.requiredItem == "none") {
                std::cout << "Special discovery:" << std::endl;
                for (const auto& item : result.loot) {
                    std::cout << "  - " << item.first << " x" << item.second << std::endl;
                    for (int i = 0; i < item.second; ++i) {
                        Item* newItem = createItem(item.first);
                        if (newItem) {
                            m_player->addItem(newItem);
                        }
                    }
                }
            } else {
                std::cout << "You need " << result.requiredItem << " to open this container." << std::endl;
                std::string requiredItem = result.requiredItem;
                if (m_player->hasItem(requiredItem)) {
                    std::cout << "You used " << requiredItem << " to open it!" << std::endl;
                    m_player->removeItem(requiredItem);
                    for (const auto& item : result.loot) {
                        std::cout << "  - " << item.first << " x" << item.second << std::endl;
                        for (int i = 0; i < item.second; ++i) {
                            Item* newItem = createItem(item.first);
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