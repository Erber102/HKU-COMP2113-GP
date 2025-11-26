#ifndef DAYPHASE_H
#define DAYPHASE_H

class Player;
class Map;
class ItemDatabase;

#include "Event.h"

class DayPhase {
private:
    Player* m_player;
    Map* m_map;
    ItemDatabase* m_itemDB;
    int m_currentDay;

    Location* selectLocationMenu();
    void exploreLocation(Location* location);
    void handleEventResult(const EventResult& result);
    void performSaveAndExit() const;

public:
    DayPhase(Player* player, Map* map, ItemDatabase* itemDB);
    void executeDay(int currentDay);
};
#endif