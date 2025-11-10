#ifndef EVENT_H
#define EVENT_H
#include "Map.h"
#include <string>
#include <vector>
#include <map>

// define the type of event
enum class EventType {
    LOOT_FOUND,      
    ENEMY_ENCOUNTER,   
    SPECIAL_DISCOVERY, 
    NOTHING_FOUND      
};

// data structure of event
struct EventResult {
    EventType type;
    std::string message; // something like "you find... a dog jumps out"
    // used in loot-find, the found item
    std::map<std::string, int> loot; 

    // used in ENEMY_ENCOUNTER: id of encountered enemy -> then proceed to combat system
    std::string enemyId;

    // used in SPECIAL_DISCOVERY: some special items
    std::string requiredItem; // e.g., "lockpick"
};

class EventFactory {
public:
    // input: location, output: event
    static EventResult generateEvent(const Location& location);
};
#endif