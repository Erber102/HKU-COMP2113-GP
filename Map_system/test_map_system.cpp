#include <iostream>
#include "Map.h"
#include "Event.h"

// 简单的测试函数，用于验证系统基本功能
void testMapSystem() {
    std::cout << "=== 测试地图系统 ===" << std::endl;

    // 测试地图生成
    Map map;
    map.generateDailyMap(1);

    auto locations = map.getAvailableLocations();
    std::cout << "生成的地点数量: " << locations.size() << std::endl;

    for (const auto* loc : locations) {
        std::cout << "地点: " << loc->name << ", 危险等级: " << loc->dangerLevel << std::endl;
    }

    std::cout << "\n=== 测试事件系统 ===" << std::endl;

    // 测试事件生成
    if (!locations.empty()) {
        EventResult result = EventFactory::generateEvent(*locations[0]);

        std::cout << "事件类型: ";
        switch (result.type) {
            case EventType::LOOT_FOUND:
                std::cout << "找到物资" << std::endl;
                break;
            case EventType::ENEMY_ENCOUNTER:
                std::cout << "遭遇敌人: " << result.enemyId << std::endl;
                break;
            case EventType::SPECIAL_DISCOVERY:
                std::cout << "特殊发现" << std::endl;
                break;
            case EventType::NOTHING_FOUND:
                std::cout << "空手而归" << std::endl;
                break;
        }
        std::cout << "消息: " << result.message << std::endl;
    }

    std::cout << "\n=== 测试完成 ===" << std::endl;
}

int main() {
    testMapSystem();
    return 0;
}