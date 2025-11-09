#ifndef MAP_H
#define MAP_H
#include <string>
#include <vector>
#include <map>

struct Location { //每个地点
    std::string id;         // 内部ID, e.g., "city_ruins"
    std::string name;       // 显示名称, "废弃的市区"
    int dangerLevel;        // 危险等级 (1-5)

    std::map<std::string, double> lootTable; //掉落东西的概率

    double enemyChance;         // 遭遇敌人的概率
    double specialEventChance;  // 特殊发现的概率
    double emptyChance;         // 空手而归的概率
};

class Map {
private:
    // 1. 这是一个"原型"列表，存储所有可能的地点模板
    std::vector<Location> locationPrototypes;

    // 2. 这是当天实际生成的地点
    std::vector<Location> dailyLocations;
    
    // 3. 追踪当天哪些地点已经被"搜刮过"
    // "走到已经探索过的节点不消耗体力/时间" -> 我们简化为"搜刮过的地点不能再搜"
    std::vector<bool> locationCompleted;

    void initializePrototypes(); // 用于加载地点模板

public:
    Map(); // 构造函数，调用 initializePrototypes

    void generateDailyMap(int currentDay); //刷新地点

    std::vector<Location*> getAvailableLocations(); // 获取可供玩家选择的地点列表 (用于显示菜单)

    void completeLocation(Location* loc); // 标记一个地点今天已经被搜刮过了
};
#endif