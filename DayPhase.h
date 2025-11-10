#ifndef DAYPHASE_H
#define DAYPHASE_H

class Player;
class Map;
class ItemDatabase;
class CombatSystem;

#include "Event.h"

class DayPhase {
private:
    Player* m_player;
    Map* m_map;
    ItemDatabase* m_itemDB;
    CombatSystem* m_combat;

    // Inner function

    // Location Menu, Player can choose a location
    Location* selectLocationMenu();
    
    // From a specific location explore nearby location
    void exploreLocation(Location* location);

    // process the result of search
    void handleEventResult(const EventResult& result);

public:
    DayPhase(Player* player, Map* map, ItemDatabase* itemDB, CombatSystem* combat);

    // core function, execute the daytime process
    void executeDay(); 
};
#endif