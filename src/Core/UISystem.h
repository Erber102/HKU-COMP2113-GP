// UISystem.h - Unified UI system for the game
#ifndef UISYSTEM_H
#define UISYSTEM_H

#include "InputSystem.h"
#include <string>
#include <vector>

// Additional color definitions for UI
#define BRIGHT_RED "\033[91m"
#define BRIGHT_GREEN "\033[92m"
#define BRIGHT_YELLOW "\033[93m"
#define BRIGHT_BLUE "\033[94m"
#define BRIGHT_MAGENTA "\033[95m"
#define BRIGHT_CYAN "\033[96m"
#define BRIGHT_WHITE "\033[97m"

// Status colors
#define STATUS_GOOD GREEN
#define STATUS_WARNING YELLOW
#define STATUS_DANGER RED
#define STATUS_INFO CYAN
#define STATUS_HIGHLIGHT BRIGHT_YELLOW

class UISystem {
public:
    // Menu functions
    static void showMenu(const std::string& title, const std::vector<std::string>& options);
    static int getMenuChoice(const std::string& title, const std::vector<std::string>& options);
    static int getNumericChoice(int min, int max, const std::string& prompt = "");

    // Display functions
    static void showStatusBar(int hp, int maxHp, int stamina, int maxStamina, int money);
    static void showInventoryHeader();
    static void showItemEntry(const std::string& itemName, int quantity, const std::string& description = "");
    static void showTradeInfo(const std::string& item, int price, int quantity, const std::string& customer = "");

    // Message functions
    static void showSuccess(const std::string& message);
    static void showWarning(const std::string& message);
    static void showError(const std::string& message);
    static void showInfo(const std::string& message);

    // Section functions
    static void startSection(const std::string& title);
    static void endSection();

    // Utility functions
    static std::string formatNumber(int number);
    static std::string formatCurrency(int amount);
    static std::string getDangerLevelColor(int dangerLevel);

    // Progress bar functions
    static std::string createProgressBar(int current, int max, int width = 20, const std::string& color = GREEN);
    static std::string createHealthBar(int current, int max, int width = 20);
    static std::string createStaminaBar(int current, int max, int width = 20);
};

#endif