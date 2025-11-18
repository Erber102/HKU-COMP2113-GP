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

class SaveSystem {
private:
    static std::string saveFileName;

    // 内部序列化方法
    static void serializePlayer(std::ofstream& file, Player& player);
    static bool deserializePlayer(std::ifstream& file, Player& player);
    
    // 物品序列化
    static void serializeInventory(std::ofstream& file, std::vector<Item*>& inventory);
    static bool deserializeInventory(std::ifstream& file, Player& player);

public:
    // 保存游戏 - 使用void*避免依赖Market
    static void saveGame(Player& player, void* market, int day);
    
    // 加载游戏 - 使用void*避免依赖Market
    static bool loadGame(Player& player, void* market, int& day);
    
    // 检查存档是否存在
    static bool saveExists();
    
    // 删除存档
    static void deleteSave();
};

#endif
