// Game.cpp
#include "Game.h"
#include <iostream>
//#include "Config.h"  
using namespace std;
// 其他模块的前向声明 - 实际开发中需要包含对应的头文件
class DayPhase {
public:
    static void executeDay(Player& player) {
        cout << "白天阶段 - 开发中..." << endl;
        // 实际实现由 pyc 完成
    }
};

class NightPhase {
public:
    static void executeNight(Player& player, void* market) {
        cout << "夜晚阶段 - 开发中..." << endl;
        
    }
};

class SaveSystem {
public:
    static bool loadGame(Player& player, void* market, int& day) {
        cout << "加载游戏 - 开发中..." << endl;
        return false;
    }
    static void saveGame(Player& player, void* market, int day) {
        cout << "保存游戏 - 开发中..." << endl;
    }
};

class Market {
    
};

Game::Game() : currentState(MAIN_MENU), currentDay(1), 
               dayPhase(nullptr), nightPhase(nullptr), 
               saveSystem(nullptr), market(nullptr) {
    initializeModules();
}

Game::~Game() {
    cleanupModules();
}

void Game::initializeModules() {
    // 这里初始化其他模块
    // 实际开发中会创建具体的实例
    cout << "初始化游戏模块..." << endl;
}

void Game::cleanupModules() {
    // 清理其他模块
    // 实际开发中会删除动态分配的对象
}

void Game::run() {
    cout << "=== GAME START ===" << endl;
    
    while (currentState != GAME_OVER) {
        switch (currentState) {
            case MAIN_MENU:
                handleMainMenu();
                break;
            case DAY:
                startDayPhase();
                break;
            case NIGHT:
                startNightPhase();
                break;
            default:
                break;
        }
        
        checkGameOver();
    }
    
    cout << "GAME OVER" << endl;
}

void Game::setState(GameState newState) {
    currentState = newState;
}

void Game::advanceDay() {
    currentDay++;
    cout << "DAY NO." << currentDay << endl;
}

void Game::startNewGame() {
    // 重置玩家状态
    player = Player();  // 使用移动语义
    currentDay = 1;
    currentState = DAY;
    cout << "NEW GAME START" << endl;
}

bool Game::loadGame() {
    Market tempMarket;
    return SaveSystem::loadGame(player, &tempMarket, currentDay);
}

void Game::saveGame() {
    Market tempMarket;
    SaveSystem::saveGame(player, &tempMarket, currentDay);
}

void Game::checkGameOver() {
    if (!player.isAlive()) {
        cout << "YOU DIE, GAME OVER" << endl;
        currentState = GAME_OVER;
    }
}

void Game::startDayPhase() {
    cout << "\n=== DAY NO." << currentDay << " : DAY TIME ===" << endl;
    
    // 调用 pyc 的模块
    DayPhase::executeDay(player);
    
    // 检查玩家是否还有体力继续
    if (player.stamina <= 0) {
        cout << "NO STAMINA LEFT" << endl;
        currentState = NIGHT;
    }
}

void Game::startNightPhase() {
    cout << "\n=== DAY NO." << currentDay << " : NIGHT TIME ===" << endl;
    
    // 显示玩家状态
    player.displayInventory();
    
    // 调用 sun 的模块 - 临时传递nullptr作为market
    NightPhase::executeNight(player, nullptr);
    
    // 进入下一天
    advanceDay();
    
    // 恢复部分体力
    player.restoreStamina(30);  
    
    // 进入下一天的白天
    currentState = DAY;
}

void Game::handleMainMenu() {
    cout << "\n=== MENUE ===" << endl;
    cout << "1. NEW GAME" << endl;
    cout << "2. LOAD GAME" << endl;
    cout << "3. EXIT" << endl;
    
    int choice;
    cout << "PLEASE CHOOSE: ";
    cin >> choice;
    
    switch (choice) {
        case 1:
            startNewGame();
            break;
        case 2:
            if (loadGame()) {
                currentState = DAY;
            }
            break;
        case 3:
            currentState = GAME_OVER;
            break;
        default:
            cout << "INVALID OPTION！" << endl;
            break;
    }
}