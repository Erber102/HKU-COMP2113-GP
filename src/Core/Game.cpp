// Game.cpp
#include "Game.h"
#include "../Map/DayPhase.h"
#include "../Night/NightPhase.h"
#include "../Save/SaveSystem.h"
#include "../Night/Market.h"
#include "../Map/Map.h"
#include "../Item/itemdatabase.h"
#include "Config.h"
#include "../Combat/combat.h"
#include "UISystem.h"
#include <iostream>
using namespace std;

// Define constants declared in Config.h
const char* SAVE_FILE_NAME = "savegame.dat";

// Constructor: Initializes game state and modules
// What it does: Sets up initial game state and calls module initialization
// Inputs: None
// Outputs: None (constructor)
Game::Game() : currentState(MAIN_MENU), currentDay(1),
               dayPhase(nullptr), nightPhase(nullptr),
               saveSystem(nullptr), market(nullptr),
               map(nullptr), itemDB(nullptr), combat(nullptr) {
    initializeModules();
}

// Destructor: Cleans up dynamically allocated modules
// What it does: Deletes all module instances to prevent memory leaks
// Inputs: None
// Outputs: None (destructor)
Game::~Game() {
    cleanupModules();
}

// Initializes all game modules and systems
// What it does: Creates instances of map, save system, market, and other core components
// Inputs: None
// Outputs: None
void Game::initializeModules() {
    // Initialize other modules
    cout << "Initializing game modules..." << endl;

    // Initialize core systems
    map = new Map();
    initItemDatabase();
    itemDB = nullptr; // ItemDatabase is a global system, no instance needed

    // Create save system instance
    saveSystem = new SaveSystem();

    // Create market instance (for night phase)
    market = new Market();

    // DayPhase will be created when actually used
    // because it needs dependencies from other modules
    dayPhase = nullptr;
    nightPhase = nullptr;
}

// Cleans up dynamically allocated modules to prevent memory leaks
// What it does: Safely deletes all module instances created during initialization
// Inputs: None
// Outputs: None
void Game::cleanupModules() {
    // Clean up dynamically allocated objects
    if (dayPhase) delete dayPhase;
    if (nightPhase) delete nightPhase;
    if (saveSystem) delete saveSystem;
    if (market) delete market;
    if (map) delete map;
    if (combat) delete combat;
}

// Main game loop that runs the entire game
// What it does: Initializes systems, handles game state transitions, and runs until game over
// Inputs: None
// Outputs: None
void Game::run() {
    InputSystem::initialize();
    UISystem::showInfo("Game initialized successfully");

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
            case COMBAT:
                startCombatPhase();
                break;
            default:
                break;
        }

        checkGameOver();
    }

    cout << "GAME OVER" << endl;
}

// Sets the current game state
// What it does: Updates the game state to control game flow between different phases
// Inputs: newState - The new game state to transition to
// Outputs: None
void Game::setState(GameState newState) {
    currentState = newState;
}

// Advances the game to the next day
// What it does: Increments day counter and displays day transition message
// Inputs: None
// Outputs: None
void Game::advanceDay() {
    currentDay++;
    UISystem::showInfo("DAY " + to_string(currentDay));
}

// Starts a new game with initial setup
// What it does: Resets player state, sets up difficulty, and initializes game
// Inputs: None
// Outputs: None
void Game::startNewGame() {
    // Reset player state
    player = Player();
    currentDay = 1;
    currentState = DAY;

    vector<string> difficultyOptions = {
        "Easy - More resources, lower demand growth",
        "Normal - Balanced experience",
        "Hard - Scarce resources, higher demand"
    };
    int difficultyChoice = UISystem::getMenuChoice("SELECT DIFFICULTY", difficultyOptions);
    int difficulty = DIFFICULTY_NORMAL;

    switch (difficultyChoice) {
        case 1:
            difficulty = DIFFICULTY_EASY;
            player.hp = EasyConfig::STARTING_HP;
            player.stamina = EasyConfig::STARTING_STAMINA;
            player.money = EasyConfig::STARTING_MONEY;
            break;
        case 2:
            difficulty = DIFFICULTY_NORMAL;
            player.hp = NormalConfig::STARTING_HP;
            player.stamina = NormalConfig::STARTING_STAMINA;
            player.money = NormalConfig::STARTING_MONEY;
            break;
        case 3:
            difficulty = DIFFICULTY_HARD;
            player.hp = HardConfig::STARTING_HP;
            player.stamina = HardConfig::STARTING_STAMINA;
            player.money = HardConfig::STARTING_MONEY;
            break;
    }
    setGameDifficulty(difficulty);

    UISystem::startSection("NEW GAME START");

    UISystem::showInfo("Welcome to the Wasteland Survival Game!");
    UISystem::showInfo("Explore during the day to collect resources, trade with customers at night to survive.");
    UISystem::showInfo("Try to survive as long as possible!");

    // Give initial items
    UISystem::showSuccess("Received initial items: Scrap x5");
    // Here you can add initial items

    UISystem::endSection();
}

// Loads a saved game from file
// What it does: Checks for save file existence and loads player data and day
// Inputs: None
// Outputs: bool - True if load successful, false otherwise
bool Game::loadGame() {
    UISystem::startSection("LOAD GAME");

    if (!SaveSystem::saveExists()) {
        UISystem::showWarning("No save file found. Please start a new game first.");
        UISystem::endSection();
        return false;
    }

    int loadedDay = 1;
    if (!SaveSystem::loadGame(player, loadedDay)) {
        UISystem::showError("Failed to load save data.");
        UISystem::endSection();
        return false;
    }

    currentDay = loadedDay;
    UISystem::showSuccess("Game loaded. Current day: " + to_string(currentDay));
    UISystem::endSection();
    return true;
}

// Saves the current game state to file
// What it does: Serializes player data and current day to save file
// Inputs: None
// Outputs: None
void Game::saveGame() {
    UISystem::startSection("SAVE GAME");
    SaveSystem::saveGame(player, currentDay);
    UISystem::endSection();
}

// Checks if game over conditions are met
// What it does: Checks if player is dead and transitions to game over state if true
// Inputs: None
// Outputs: None
void Game::checkGameOver() {
    if (!player.isAlive()) {
        UISystem::startSection("GAME OVER");
        UISystem::showError("YOU DIED");
        UISystem::showInfo("Better luck next time!");
        UISystem::endSection();
        currentState = GAME_OVER;
    }
}

// Starts the day phase of the game
// What it does: Generates daily map and executes day phase activities
// Inputs: None
// Outputs: None
void Game::startDayPhase() {
    cout << "\n=== DAY " << currentDay << " : DAY TIME ===" << endl;

    // Generate daily map
    map->generateDailyMap(currentDay);

    // Create DayPhase instance if not exists
    if (!dayPhase) {
        dayPhase = new DayPhase(&player, map, itemDB);
    }

    dayPhase->executeDay(currentDay);

    // After day phase, automatically transition to night
    currentState = NIGHT;
}

// Starts the night phase of the game
// What it does: Displays player status and executes night phase trading activities
// Inputs: None
// Outputs: None
void Game::startNightPhase() {
    cout << "\n=== DAY " << currentDay << " : NIGHT TIME ===" << endl;

    // Display player status
    player.displayInventory();

    // Use our created market instance
    NightPhase::executeNight(player, market, currentDay);

    // Advance to next day
    advanceDay();

    // Restore some stamina
    player.restoreStamina(30);

    // Enter next day's daytime
    currentState = DAY;
}

// Starts the combat phase of the game
// What it does: Initializes combat player and starts combat with enemies
// Inputs: None
// Outputs: None
void Game::startCombatPhase() {
    cout << "\n=== COMBAT PHASE ===" << endl;

    // Initialize combat player if not already done
    if (!player.getCombatPlayer()) {
        player.initializeCombatPlayer("Player");
    }

    Item* bestWeapon = player.getBestWeapon();

    // Create combat instance
    if (!combat) {
        combat = new Combat(&player, "Player", bestWeapon);
    } else {
        combat->equipWeapon(bestWeapon);
    }

    // Start combat with first enemy
    combat->fightEnemy(0);

    // After combat, return to day phase
    currentState = DAY;
}

// Handles the main menu interface and user selections
// What it does: Displays main menu options and processes user choices
// Inputs: None
// Outputs: None
void Game::handleMainMenu() {
    vector<string> menuOptions = {
        "Start a New Game",
        "Load Saved Game",
        "Test Combat System",
        "Exit Game"
    };

    int choice = UISystem::getMenuChoice("WASTELAND SURVIVAL GAME", menuOptions);

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
            currentState = COMBAT;
            break;
        case 4:
            currentState = GAME_OVER;
            break;
        default:
            UISystem::showError("Invalid option selected!");
            break;
    }
}