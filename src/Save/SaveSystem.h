#ifndef SAVESYSTEM_H
#define SAVESYSTEM_H

#include "../Core/Common.h"
#include "../Player/Player.h"
#include "../Item/item.h"
#include "../Item/itemdatabase.h"
#include "../Core/Config.h"
#include <string>
#include <fstream>
#include <vector>

// MarketTrends结构体 - 在Market.h中定义

class SaveSystem {
private:
    static std::string saveFileName;

    // 内部序列化方法
    static void serializePlayer(std::ofstream& file, Player& player);
    static bool deserializePlayer(std::ifstream& file, Player& player);
    
    // 物品序列化
    static void serializeInventory(std::ofstream& file, const std::vector<std::unique_ptr<Item>>& inventory);
    static bool deserializeInventory(std::ifstream& file, Player& player);
    
    // MarketTrends序列化 - 暂时移除

public:
    // 保存游戏
    static void saveGame(Player& player, int day);

    // 加载游戏
    static bool loadGame(Player& player, int& day);
    
    // 检查存档是否存在
    static bool saveExists();
    
    // 删除存档
    static void deleteSave();
    
    // 需求增长函数 - 暂时移除
};

#endif
