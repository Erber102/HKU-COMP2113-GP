#include "SaveSystem.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

string SaveSystem::saveFileName = SAVE_FILE_NAME;

void SaveSystem::serializeMarketTrends(ofstream& file, MarketTrends& trends) {
    file.write(reinterpret_cast<const char*>(&trends.food_demand), sizeof(trends.food_demand));
    file.write(reinterpret_cast<const char*>(&trends.water_demand), sizeof(trends.water_demand));
    file.write(reinterpret_cast<const char*>(&trends.medicine_demand), sizeof(trends.medicine_demand));
    file.write(reinterpret_cast<const char*>(&trends.metal_demand), sizeof(trends.metal_demand));
    file.write(reinterpret_cast<const char*>(&trends.currentDifficulty), sizeof(trends.currentDifficulty));
}

bool SaveSystem::deserializeMarketTrends(ifstream& file, MarketTrends& trends) {
    file.read(reinterpret_cast<char*>(&trends.food_demand), sizeof(trends.food_demand));
    file.read(reinterpret_cast<char*>(&trends.water_demand), sizeof(trends.water_demand));
    file.read(reinterpret_cast<char*>(&trends.medicine_demand), sizeof(trends.medicine_demand));
    file.read(reinterpret_cast<char*>(&trends.metal_demand), sizeof(trends.metal_demand));
    file.read(reinterpret_cast<char*>(&trends.currentDifficulty), sizeof(trends.currentDifficulty));
    
    // 验证数据合理性
    if (file.fail() || 
        trends.food_demand < 0.1f || trends.food_demand > 100.0f ||
        trends.currentDifficulty < DIFFICULTY_EASY || trends.currentDifficulty > DIFFICULTY_HARD) {
        return false;
    }
    
    return true;
}

void SaveSystem::updateMarketTrends(MarketTrends& trends) {
    trends.updateTrends();
}

void SaveSystem::saveGame(Player& player, MarketTrends& trends, int day) {
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
        
        // 保存MarketTrends数据
        serializeMarketTrends(file, trends);
        
        file.close();
        cout << "Game saved successfully! Day " << day << endl;
        
    } catch (const exception& e) {
        cerr << "Error saving game: " << e.what() << endl;
        if (file.is_open()) {
            file.close();
        }
        deleteSave();
    }
}

bool SaveSystem::loadGame(Player& player, MarketTrends& trends, int& day) {
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
        
        // 加载MarketTrends数据
        if (!deserializeMarketTrends(file, trends)) {
            cerr << "Error: Corrupted market data!" << endl;
            file.close();
            return false;
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
