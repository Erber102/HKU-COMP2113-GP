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

Game::Game() : currentState(MAIN_MENU), currentDay(1),
               dayPhase(nullptr), nightPhase(nullptr),
               saveSystem(nullptr), market(nullptr),
               map(nullptr), itemDB(nullptr), combat(nullptr) {
    initializeModules();
}

Game::~Game() {
    cleanupModules();
}

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

void Game::cleanupModules() {
    // Clean up dynamically allocated objects
    if (dayPhase) delete dayPhase;
    if (nightPhase) delete nightPhase;
    if (saveSystem) delete saveSystem;
    if (market) delete market;
    if (map) delete map;
    if (combat) delete combat;
}

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

void Game::setState(GameState newState) {
    currentState = newState;
}

void Game::advanceDay() {
    currentDay++;
    UISystem::showInfo("DAY " + to_string(currentDay));
}

void Game::startNewGame() {
    // Reset player state
    player = Player();
    currentDay = 1;
    currentState = DAY;

    UISystem::startSection("NEW GAME START");

    UISystem::showInfo("Welcome to the Wasteland Survival Game!");
    UISystem::showInfo("Explore during the day to collect resources, trade with customers at night to survive.");
    UISystem::showInfo("Try to survive as long as possible!");

    // Give initial items
    UISystem::showSuccess("Received initial items: Scrap x5");
    // Here you can add initial items

    UISystem::endSection();
}

bool Game::loadGame() {
    // Simplified load game
    UISystem::showWarning("Load game feature - Under development...");
    return false;
}

void Game::saveGame() {
    // Simplified save game
    UISystem::showWarning("Save game feature - Under development...");
}

void Game::checkGameOver() {
    if (!player.isAlive()) {
        UISystem::startSection("GAME OVER");
        UISystem::showError("YOU DIED");
        UISystem::showInfo("Better luck next time!");
        UISystem::endSection();
        currentState = GAME_OVER;
    }
}

void Game::startDayPhase() {
    cout << "\n=== DAY " << currentDay << " : DAY TIME ===" << endl;

    // Generate daily map
    map->generateDailyMap(currentDay);

    // Create DayPhase instance if not exists
    if (!dayPhase) {
        dayPhase = new DayPhase(&player, map, itemDB);
    }

    // Execute the day phase using the full map system
    dayPhase->executeDay();

    // After day phase, automatically transition to night
    currentState = NIGHT;
}

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

void Game::startCombatPhase() {
    cout << "\n=== COMBAT PHASE ===" << endl;

    // Initialize combat player if not already done
    if (!player.getCombatPlayer()) {
        player.initializeCombatPlayer("Player");
    }

    // Create combat instance
    if (!combat) {
        combat = new Combat("Player");
    }

    // Start combat with first enemy
    combat->fightEnemy(0);

    // After combat, return to day phase
    currentState = DAY;
}

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