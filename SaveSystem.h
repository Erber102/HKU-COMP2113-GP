#ifndef SAVESYSTEM_H
#define SAVESYSTEM_H

#include "Common.h"
#include "Player.h"
#include "item.h"
#include "itemdatabase.h"
#include "Config.h"
#include <string>
#include <fstream>
#include <vector>

// MarketTrends结构体 - 实现需求增长系统
struct MarketTrends {
    float food_demand = 1.0f;
    float water_demand = 1.0f; 
    float medicine_demand = 1.0f;
    float metal_demand = 1.0f;
    int currentDifficulty = DIFFICULTY_NORMAL;
    
    // 需求增长函数
    void updateTrends() {
        food_demand *= DEMAND_GROWTH_RATE;
        water_demand *= DEMAND_GROWTH_RATE;
        medicine_demand *= DEMAND_GROWTH_RATE;
        metal_demand *= DEMAND_GROWTH_RATE;
    }
};

class SaveSystem {
private:
    static std::string saveFileName;

    // 内部序列化方法
    static void serializePlayer(std::ofstream& file, Player& player);
    static bool deserializePlayer(std::ifstream& file, Player& player);
    
    // 物品序列化
    static void serializeInventory(std::ofstream& file, std::vector<Item*>& inventory);
    static bool deserializeInventory(std::ifstream& file, Player& player);
    
    // MarketTrends序列化
    static void serializeMarketTrends(std::ofstream& file, MarketTrends& trends);
    static bool deserializeMarketTrends(std::ifstream& file, MarketTrends& trends);

public:
    // 保存游戏 - 现在包含MarketTrends
    static void saveGame(Player& player, MarketTrends& trends, int day);
    
    // 加载游戏 - 现在包含MarketTrends  
    static bool loadGame(Player& player, MarketTrends& trends, int& day);
    
    // 检查存档是否存在
    static bool saveExists();
    
    // 删除存档
    static void deleteSave();
    
    // 需求增长函数
    static void updateMarketTrends(MarketTrends& trends);
};

#endif
