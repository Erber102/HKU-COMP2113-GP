#ifndef CONFIG_H
#define CONFIG_H

enum Difficulty {
    DIFFICULTY_EASY = 0,
    DIFFICULTY_NORMAL = 1,
    DIFFICULTY_HARD = 2
};

namespace EasyConfig {
    const int STARTING_HP = 120;
    const int STARTING_STAMINA = 70;
    const int STARTING_MONEY = 50;
    const float DEMAND_GROWTH_RATE = 1.03;
}

namespace NormalConfig {
    const int STARTING_HP = 100;
    const int STARTING_STAMINA = 50;
    const int STARTING_MONEY = 0;
    const float DEMAND_GROWTH_RATE = 1.05;
}

namespace HardConfig {
    const int STARTING_HP = 80;
    const int STARTING_STAMINA = 30;
    const int STARTING_MONEY = -20;
    const float DEMAND_GROWTH_RATE = 1.08;
}

extern const char* SAVE_FILE_NAME;

const int BASE_STAMINA_RESTORE = 30;
const int MAX_STAMINA = 100;

#endif
