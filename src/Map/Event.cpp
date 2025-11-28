#include "Event.h"
#include <iostream>
#include <random>
#include <map>
#include <cstdlib>
#include <iterator>
#include <algorithm>

EventResult EventFactory::generateEvent(const Location& location) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    double roll = dis(gen);
    double cumulative = 0.0;

    EventResult result;

    // Determine event type based on location probabilities
    double adjustedEmptyChance = std::max(0.05, location.emptyChance * 0.5);
    cumulative += adjustedEmptyChance;
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
            for (const auto& loot : location.lootTable) {
                double chance = std::min(0.95, loot.second * 2.5);
                if (dis(gen) < chance) {
                    int quantity = 1 + (location.dangerLevel / 2);
                    result.loot[loot.first] = quantity;
                }
            }
            if (result.loot.empty()) {
                result.loot["medkit"] = 1 + (location.dangerLevel / 3);
            }
        }
        return result;
    }

    // Default event: find loot
    result.type = EventType::LOOT_FOUND;
    result.message = "You found some useful supplies in the ruins!";

    // Generate loot based on loot table
    for (const auto& loot : location.lootTable) {
        double boostedChance = std::min(0.95, loot.second + location.dangerLevel * 0.1);
        if (dis(gen) < boostedChance) {
            // Higher danger levels yield more items
            int quantity = 1 + (location.dangerLevel / 3);
            result.loot[loot.first] = quantity;
        }
    }

    double scrapChance = std::min(0.9, 0.25 + location.dangerLevel * 0.15);
    if (dis(gen) < scrapChance) {
        int scrapQty = 1 + (location.dangerLevel >= 3 ? rand() % 2 : 0);
        result.loot["Scrap Metal"] += scrapQty;
    }
    if (dis(gen) < scrapChance * 0.5f) {
        result.loot["Rag"] += 1;
    }

    // If nothing was found, change to nothing found
    if (result.loot.empty()) {
        if (!location.lootTable.empty()) {
            auto it = location.lootTable.begin();
            std::advance(it, rand() % location.lootTable.size());
            result.loot[it->first] = 1;
            result.type = EventType::LOOT_FOUND;
            result.message = "You almost left empty-handed but found a small supply.";
        } else {
            result.type = EventType::NOTHING_FOUND;
            result.message = "You searched the area but only found useless junk...";
        }
    }

    return result;
}