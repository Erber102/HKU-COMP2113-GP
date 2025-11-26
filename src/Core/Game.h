// Game.h
#ifndef GAME_H
#define GAME_H

#include "Common.h"
#include "Player.h"

// 前向声明其他模块
class DayPhase;
class NightPhase;
class SaveSystem;
class Market;
class Map;
class ItemDatabase;
class Combat;

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
    Map* map;
    ItemDatabase* itemDB;
    Combat* combat;

    // 初始化方法
    void initializeModules();
    void cleanupModules();

public:
    // 构造函数和析构函数
    Game();
    ~Game();

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

    // 游戏流程控制
    void startNewGame();
    bool loadGame();
    void saveGame();
    void checkGameOver();

    // 阶段切换
    void startDayPhase();
    void startNightPhase();
    void startCombatPhase();
    void handleMainMenu();
};

#endif 