#ifndef CONFIG_H
#define CONFIG_H

/**
 * @brief Difficulty levels for the game
 * 
 * Defines the available difficulty settings that affect game parameters
 */
enum Difficulty {
    DIFFICULTY_EASY = 0,    ///< Easy difficulty mode
    DIFFICULTY_NORMAL = 1,  ///< Normal difficulty mode
    DIFFICULTY_HARD = 2     ///< Hard difficulty mode
};

/**
 * @brief Configuration values for Easy difficulty
 * 
 * Contains all starting parameters and rates for Easy difficulty mode
 */
namespace EasyConfig {
    const int STARTING_HP = 120;                ///< Starting health points for player
    const int STARTING_STAMINA = 70;            ///< Starting stamina points for player
    const int STARTING_MONEY = 50;              ///< Starting currency amount
    const float DEMAND_GROWTH_RATE = 1.03;      ///< Rate at which market demand increases
}

/**
 * @brief Configuration values for Normal difficulty
 * 
 * Contains all starting parameters and rates for Normal difficulty mode
 */
namespace NormalConfig {
    const int STARTING_HP = 100;                ///< Starting health points for player
    const int STARTING_STAMINA = 50;            ///< Starting stamina points for player
    const int STARTING_MONEY = 0;               ///< Starting currency amount
    const float DEMAND_GROWTH_RATE = 1.05;      ///< Rate at which market demand increases
}

/**
 * @brief Configuration values for Hard difficulty
 * 
 * Contains all starting parameters and rates for Hard difficulty mode
 */
namespace HardConfig {
    const int STARTING_HP = 80;                 ///< Starting health points for player
    const int STARTING_STAMINA = 30;            ///< Starting stamina points for player
    const int STARTING_MONEY = -20;             ///< Starting currency amount (debt)
    const float DEMAND_GROWTH_RATE = 1.08;      ///< Rate at which market demand increases
}

/**
 * @brief Name of the save file used for game progress
 * 
 * External declaration for the save file name constant
 */
extern const char* SAVE_FILE_NAME;

/**
 * @brief Base stamina restoration amount
 * 
 * Amount of stamina restored when player rests (base value)
 */
const int BASE_STAMINA_RESTORE = 30;

/**
 * @brief Maximum possible stamina value
 * 
 * Upper limit for player's stamina points
 */
const int MAX_STAMINA = 100;

#endif
