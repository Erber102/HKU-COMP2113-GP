#ifndef CONFIG_H
#define CONFIG_H

// 难度分级
enum Difficulty {
    DIFFICULTY_EASY = 0,
    DIFFICULTY_NORMAL = 1,
    DIFFICULTY_HARD = 2
};

// 难度配置常量 - 根据不同难度设置不同数值
namespace EasyConfig {
    const int STARTING_HP = 120;
    const int STARTING_STAMINA = 70;
    const int STARTING_MONEY = 50;
    const double DEMAND_GROWTH_RATE = 1.03;
}

namespace NormalConfig {
    const int STARTING_HP = 100;
    const int STARTING_STAMINA = 50;
    const int STARTING_MONEY = 0;
    const double DEMAND_GROWTH_RATE = 1.05;
}

namespace HardConfig {
    const int STARTING_HP = 80;
    const int STARTING_STAMINA = 30;
    const int STARTING_MONEY = -20; // 初始负债
    const double DEMAND_GROWTH_RATE = 1.08;
}

// 存档相关配置
extern const char* SAVE_FILE_NAME;

// 游戏平衡常数
const int BASE_STAMINA_RESTORE = 30;
const int MAX_STAMINA = 100;

#endif
