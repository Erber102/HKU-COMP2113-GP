#ifndef EVENT_H
#define EVENT_H
#include "Map.h"
#include <string>
#include <vector>
#include <map>

// 定义事件的类型
enum class EventType {
    LOOT_FOUND,      
    ENEMY_ENCOUNTER,   
    SPECIAL_DISCOVERY, 
    NOTHING_FOUND      
};

// "事件" 的数据结构
struct EventResult {
    EventType type;
    std::string message; // "你找到了..." / "一只狗跳了出来！"

    // 仅用于 LOOT_FOUND: 找到的物品ID和数量
    std::map<std::string, int> loot; 

    // 仅用于 ENEMY_ENCOUNTER: 遭遇的敌人ID (交给战斗系统处理)
    std::string enemyId;

    // 仅用于 SPECIAL_DISCOVERY: 需要的道具ID
    std::string requiredItem; // e.g., "lockpick"
};

// 静态工厂类
class EventFactory {
public:
    // 输入一个地点，输出一个随机事件
    static EventResult generateEvent(const Location& location);
};
#endif