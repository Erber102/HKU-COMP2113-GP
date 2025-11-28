#include "Map.h"
#include <iostream>
#include <random>
#include <algorithm>

// Constructor: Initializes map with location prototypes
// What it does: Creates map instance and sets up location templates
// Inputs: None
// Outputs: None (constructor)
Map::Map() {
    initializePrototypes();
}

// Initializes location prototypes with different danger levels
// What it does: Sets up predefined location templates with items and probabilities
// Inputs: None
// Outputs: None
void Map::initializePrototypes() {
    // Define location templates with different danger levels
    locationPrototypes = {
        // Low danger areas
        {"safe_zone", "Safe Zone", 1,
         {{"bandage", 0.3}, {"water", 0.4}, {"canned_food", 0.2}},
         0.1, 0.05, 0.5},

        // Medium danger areas
        {"suburbs", "Suburbs", 2,
         {{"bandage", 0.2}, {"water", 0.3}, {"canned_food", 0.3}, {"pistol", 0.05}},
         0.3, 0.1, 0.3},

        // High danger areas
        {"city_ruins", "City Ruins", 4,
         {{"medkit", 0.1}, {"rifle", 0.08}, {"grenade", 0.05}, {"energy_drink", 0.2}},
         0.6, 0.15, 0.1},

        // Very high danger areas
        {"military_base", "Military Base", 5,
         {{"medkit", 0.15}, {"sniper_rifle", 0.05}, {"grenade", 0.1}, {"armor", 0.08}},
         0.8, 0.2, 0.05}
    };
}

// Generates daily map with locations based on current day
// What it does: Creates random locations with adjusted danger levels for the day
// Inputs: currentDay - Current day number for difficulty scaling
// Outputs: None
void Map::generateDailyMap(int currentDay) {
    dailyLocations.clear();
    locationCompleted.clear();

    std::random_device rd;
    std::mt19937 gen(rd());

    // Increase map complexity and danger based on day
    int locationCount = 3 + (currentDay / 3); // Add one location every 3 days
    locationCount = std::min(locationCount, 8); // Max 8 locations

    // Generate daily map
    for (int i = 0; i < locationCount; ++i) {
        std::uniform_int_distribution<> dis(0, locationPrototypes.size() - 1);
        Location location = locationPrototypes[dis(gen)];

        // Adjust danger level based on day
        int adjustedDanger = location.dangerLevel + (currentDay / 5);
        location.dangerLevel = std::min(adjustedDanger, 5);

        // Add unique ID
        location.id = location.id + "_" + std::to_string(i);

        dailyLocations.push_back(location);
        locationCompleted.push_back(false);
    }
}

// Gets list of available (uncompleted) locations
// What it does: Returns pointers to locations that haven't been completed yet
// Inputs: None
// Outputs: vector<Location*> - List of available location pointers
std::vector<Location*> Map::getAvailableLocations() {
    std::vector<Location*> available;

    for (size_t i = 0; i < dailyLocations.size(); ++i) {
        if (!locationCompleted[i]) {
            available.push_back(&dailyLocations[i]);
        }
    }

    return available;
}

// Marks a location as completed
// What it does: Sets location completion status to true for the specified location
// Inputs: loc - Pointer to location to mark as completed
// Outputs: None
void Map::completeLocation(Location* loc) {
    for (size_t i = 0; i < dailyLocations.size(); ++i) {
        if (&dailyLocations[i] == loc) {
            locationCompleted[i] = true;
            break;
        }
    }
}