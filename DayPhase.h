#ifndef DAYPHASE_H
#define DAYPHASE_H

class Player;
class Map;
class ItemDatabase;
class CombatSystem;

#include "Event.h" // 需要 EventResult

class DayPhase {
private:
    // 持有指向核心系统的指针 (由 Game 主类传入)
    Player* m_player;
    Map* m_map;
    ItemDatabase* m_itemDB;
    CombatSystem* m_combat;

    // --- 内部辅助函数 ---

    // 1. 显示地点菜单，并让玩家选择
    Location* selectLocationMenu();
    
    // 2. 在选定的地点循环"搜索"
    void exploreLocation(Location* location);

    // 3. 处理"搜索"出的事件结果
    void handleEventResult(const EventResult& result);

public:
    // 构造函数，用于"依赖注入"
    DayPhase(Player* player, Map* map, ItemDatabase* itemDB, CombatSystem* combat);

    // 核心功能：执行一整天的逻辑
    void executeDay(); 
};
#endif