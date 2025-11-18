#include "SaveSystem.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

string SaveSystem::saveFileName = SAVE_FILE_NAME;

void SaveSystem::serializeInventory(ofstream& file, vector<Item*>& inventory) {
    // 统计相同物品的数量
    vector<pair<string, int>> itemCounts;
    for (Item* item : inventory) {
        bool found = false;
        for (auto& pair : itemCounts) {
            if (pair.first == item->name) {
                pair.second++;
                found = true;
                break;
            }
        }
        if (!found) {
            itemCounts.push_back({item->name, 1});
        }
    }
    
    // 写入物品统计数量
    int uniqueItemCount = itemCounts.size();
    file.write(reinterpret_cast<const char*>(&uniqueItemCount), sizeof(uniqueItemCount));
    
    // 写入每个物品的名称和数量
    for (auto& pair : itemCounts) {
        // 写入物品名称长度和名称
        int nameLength = pair.first.length();
        file.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
        file.write(pair.first.c_str(), nameLength);
        
        // 写入物品数量
        file.write(reinterpret_cast<const char*>(&pair.second), sizeof(pair.second));
    }
}

bool SaveSystem::deserializeInventory(ifstream& file, Player& player) {
    // 读取唯一物品数量
    int uniqueItemCount;
    file.read(reinterpret_cast<char*>(&uniqueItemCount), sizeof(uniqueItemCount));
    
    if (file.fail() || uniqueItemCount < 0 || uniqueItemCount > 1000) {
        return false; // 数据损坏
    }
    
    // 重新创建物品栏
    for (int i = 0; i < uniqueItemCount; i++) {
        // 读取物品名称长度
        int nameLength;
        file.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
        
        if (file.fail() || nameLength <= 0 || nameLength > 100) {
            return false; // 数据损坏
        }
        
        // 读取物品名称
        string itemName;
        itemName.resize(nameLength);
        file.read(&itemName[0], nameLength);
        
        if (file.fail()) {
            return false;
        }
        
        // 读取物品数量
        int quantity;
        file.read(reinterpret_cast<char*>(&quantity), sizeof(quantity));
        
        if (file.fail() || quantity <= 0 || quantity > 100) {
            return false; // 数据损坏
        }
        
        // 使用ItemDatabase重新创建物品
        for (int j = 0; j < quantity; j++) {
            Item* newItem = createItem(itemName);
            if (newItem != nullptr) {
                player.addItem(newItem);
            } else {
                cerr << "Warning: Item '" << itemName << "' not found in database!" << endl;
                // 继续加载其他物品，不中断
            }
        }
    }
    
    return true;
}

void SaveSystem::serializePlayer(ofstream& file, Player& player) {
    // 保存玩家基础状态
    file.write(reinterpret_cast<const char*>(&player.hp), sizeof(player.hp));
    file.write(reinterpret_cast<const char*>(&player.stamina), sizeof(player.stamina));
    file.write(reinterpret_cast<const char*>(&player.money), sizeof(player.money));
    
    // 保存物品栏
    serializeInventory(file, player.getInventory());
}

bool SaveSystem::deserializePlayer(ifstream& file, Player& player) {
    // 清空玩家当前状态
    player.clearInventory();
    
    // 读取玩家基础状态
    file.read(reinterpret_cast<char*>(&player.hp), sizeof(player.hp));
    file.read(reinterpret_cast<char*>(&player.stamina), sizeof(player.stamina));
    file.read(reinterpret_cast<char*>(&player.money), sizeof(player.money));
    
    if (file.fail()) {
        return false;
    }
    
    // 验证数据合理性
    if (player.hp < 0 || player.hp > 1000 || 
        player.stamina < 0 || player.stamina > 1000 || 
        player.money < 0 || player.money > 1000000) {
        return false;
    }
    
    // 读取物品栏
    return deserializeInventory(file, player);
}

void SaveSystem::saveGame(Player& player, void* market, int day) {
    ofstream file(saveFileName, ios::binary);
    
    if (!file.is_open()) {
        cerr << "Error: Cannot open save file for writing!" << endl;
        return;
    }
    
    try {
        // 写入文件头标识
        const char* header = "SAVEGAME";
        file.write(header, 8);
        
        // 写入版本号
        int version = 1;
        file.write(reinterpret_cast<const char*>(&version), sizeof(version));
        
        // 保存当前天数
        file.write(reinterpret_cast<const char*>(&day), sizeof(day));
        
        // 保存玩家数据
        serializePlayer(file, player);
        
        // Market数据暂时留空，等Market模块完成后可以扩展
        // 这里可以预留位置给market数据
        
        file.close();
        cout << "Game saved successfully! Day " << day << endl;
        
    } catch (const exception& e) {
        cerr << "Error saving game: " << e.what() << endl;
        if (file.is_open()) {
            file.close();
        }
        // 删除损坏的存档文件
        deleteSave();
    }
}

bool SaveSystem::loadGame(Player& player, void* market, int& day) {
    ifstream file(saveFileName, ios::binary);
    
    if (!file.is_open()) {
        cerr << "Error: Save file not found!" << endl;
        return false;
    }
    
    try {
        // 验证文件头
        char header[9] = {0};
        file.read(header, 8);
        if (string(header) != "SAVEGAME") {
            cerr << "Error: Invalid save file format!" << endl;
            file.close();
            return false;
        }
        
        // 读取版本号
        int version;
        file.read(reinterpret_cast<char*>(&version), sizeof(version));
        if (version != 1) {
            cerr << "Error: Unsupported save file version!" << endl;
            file.close();
            return false;
        }
        
        // 读取当前天数
        file.read(reinterpret_cast<char*>(&day), sizeof(day));
        if (file.fail() || day < 1 || day > 1000) {
            cerr << "Error: Corrupted save data (day)!" << endl;
            file.close();
            return false;
        }
        
        // 加载玩家数据
        if (!deserializePlayer(file, player)) {
            cerr << "Error: Corrupted player data!" << endl;
            file.close();
            return false;
        }
        
        // 检查文件是否读取完整
        if (file.peek() != EOF) {
            cerr << "Warning: Extra data in save file!" << endl;
        }
        
        file.close();
        cout << "Game loaded successfully! Day " << day << endl;
        return true;
        
    } catch (const exception& e) {
        cerr << "Error loading game: " << e.what() << endl;
        if (file.is_open()) {
            file.close();
        }
        return false;
    }
}

bool SaveSystem::saveExists() {
    ifstream file(saveFileName);
    bool exists = file.good();
    file.close();
    return exists;
}

void SaveSystem::deleteSave() {
    if (remove(saveFileName.c_str()) == 0) {
        cout << "Save file deleted." << endl;
    } else {
        cerr << "Error deleting save file!" << endl;
    }
}
