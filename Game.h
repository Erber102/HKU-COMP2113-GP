// Game.h
#ifndef GAME_H
#define GAME_H

#include "Common.h"
#include "Player.h"
using namespace std;

// 前向声明其他模块
class DayPhase;
class NightPhase;
class SaveSystem;
class Market;

class Game {
private:
    Player player;
    GameState currentState;
    int currentDay;
    
    // 其他模块的指针
    DayPhase* dayPhase;
    NightPhase* nightPhase;
    SaveSystem* saveSystem;
    Market* market;
    
    // 初始化方法
    void initializeModules();
    void cleanupModules();
    
public:
    
    // 主游戏循环
    void run();
    
    // 状态管理
    void setState(GameState newState);
    GameState getState() { return currentState; }
    
    // 天数管理
    void advanceDay();
    int getCurrentDay() { return currentDay; }
    
    // 玩家访问
    Player& getPlayer() { return player; }
    Player& getPlayer() { return player; }
    
    // 游戏流程控制
    void startNewGame();
    bool loadGame();
    void saveGame();
    void checkGameOver();
    
    // 阶段切换
    void startDayPhase();
    void startNightPhase();
    void handleMainMenu();
};

#endif 