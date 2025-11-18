#include "Event.h"
#include <iostream>
#include <random>
#include <map>

EventResult EventFactory::generateEvent(const Location& location) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    double roll = dis(gen);
    double cumulative = 0.0;

    EventResult result;

    // Determine event type based on location probabilities
    cumulative += location.emptyChance;
    if (roll < cumulative) {
        result.type = EventType::NOTHING_FOUND;
        result.message = "You searched carefully but found nothing...";
        return result;
    }

    cumulative += location.enemyChance;
    if (roll < cumulative) {
        result.type = EventType::ENEMY_ENCOUNTER;

        // Determine enemy type based on danger level
        std::map<int, std::string> enemyTypes = {
            {1, "zombie"},
            {2, "mutant_dog"},
            {3, "raider"},
            {4, "mutant_beast"},
            {5, "boss"}
        };

        result.enemyId = enemyTypes[location.dangerLevel];
        result.message = "Danger! " + std::string(location.dangerLevel >= 4 ? "Powerful " : "") +
                        "enemy jumped out from the shadows!";
        return result;
    }

    cumulative += location.specialEventChance;
    if (roll < cumulative) {
        result.type = EventType::SPECIAL_DISCOVERY;

        // Special discoveries require specific items
        std::map<int, std::string> specialItems = {
            {1, "none"}, // Low danger areas don't need special items
            {2, "lockpick"},
            {3, "crowbar"},
            {4, "explosives"},
            {5, "master_key"}
        };

        result.requiredItem = specialItems[location.dangerLevel];

        if (result.requiredItem == "none") {
            result.message = "You found a hidden treasure chest! It contains valuable supplies.";
            // Generate better loot
            for (const auto& loot : location.lootTable) {
                if (dis(gen) < loot.second * 2) { // Double probability
                    result.loot[loot.first] = 1 + (location.dangerLevel / 2);
                }
            }
        } else {
            result.message = "You found a locked container that requires " + result.requiredItem + " to open.";
        }
        return result;
    }

    // Default event: find loot
    result.type = EventType::LOOT_FOUND;
    result.message = "You found some useful supplies in the ruins!";

    // Generate loot based on loot table
    for (const auto& loot : location.lootTable) {
        if (dis(gen) < loot.second) {
            // Higher danger levels yield more items
            int quantity = 1 + (location.dangerLevel / 3);
            result.loot[loot.first] = quantity;
        }
    }

    // If nothing was found, change to nothing found
    if (result.loot.empty()) {
        result.type = EventType::NOTHING_FOUND;
        result.message = "You searched the area but only found useless junk...";
    }

    return result;
}