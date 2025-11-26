// UISystem.cpp - Unified UI system implementation
#include "UISystem.h"
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

void UISystem::showMenu(const string& title, const vector<string>& options) {
    InputSystem::clearScreen();
    InputSystem::drawTitle(title);
    InputSystem::drawSeparator();

    for (size_t i = 0; i < options.size(); ++i) {
        cout << " " << BRIGHT_CYAN << "[" << (i + 1) << "]" << RESET << " " << options[i] << endl;
    }

    InputSystem::drawSeparator();
}

int UISystem::getMenuChoice(const string& title, const vector<string>& options) {
    showMenu(title, options);
    cout << STATUS_INFO << "Use number keys to select an option" << RESET << endl;

    while (true) {
        if (InputSystem::kbhit()) {
            char key = InputSystem::getch();
            int choice = key - '0';

            if (choice >= 1 && choice <= static_cast<int>(options.size())) {
                return choice;
            } else {
                cout << STATUS_WARNING << "Invalid choice! Please select 1-" << options.size() << RESET << endl;
            }
        }
        InputSystem::sleepMs(100);
    }
}

int UISystem::getNumericChoice(int min, int max, const string& prompt) {
    if (!prompt.empty()) {
        cout << STATUS_INFO << prompt << RESET << endl;
    }

    while (true) {
        if (InputSystem::kbhit()) {
            char key = InputSystem::getch();

            if (key == 'q' || key == 'Q') {
                return -1; // Quit signal
            }

            if (isdigit(key)) {
                int choice = key - '0';

                // Handle multi-digit numbers
                if (choice >= min && choice <= max) {
                    return choice;
                } else {
                    cout << STATUS_WARNING << "Please enter a number between " << min << " and " << max << RESET << endl;
                }
            } else {
                cout << STATUS_WARNING << "Please enter a valid number" << RESET << endl;
            }
        }
        InputSystem::sleepMs(100);
    }
}

void UISystem::showStatusBar(int hp, int maxHp, int stamina, int maxStamina, int money) {
    cout << BOLD << "=== PLAYER STATUS ===" << RESET << endl;

    string healthColor = (hp > maxHp * 0.6) ? GREEN : (hp > maxHp * 0.3) ? YELLOW : RED;
    string staminaColor = (stamina > maxStamina * 0.5) ? GREEN : (stamina > maxStamina * 0.2) ? YELLOW : RED;

    cout << "Health:   " << healthColor << createHealthBar(hp, maxHp) << " " << hp << "/" << maxHp << " HP" << RESET << endl;
    cout << "Stamina:  " << staminaColor << createStaminaBar(stamina, maxStamina) << " " << stamina << "/" << maxStamina << RESET << endl;
    cout << "Money:    " << BRIGHT_YELLOW << formatCurrency(money) << RESET << endl;

    InputSystem::drawSeparator();
}

void UISystem::showInventoryHeader() {
    cout << BOLD << BRIGHT_CYAN << "=== INVENTORY ===" << RESET << endl;
}

void UISystem::showItemEntry(const string& itemName, int quantity, const string& description) {
    string quantityColor = (quantity > 0) ? GREEN : RED;

    cout << "  " << BRIGHT_WHITE << itemName << RESET;
    cout << " x" << quantityColor << quantity << RESET;

    if (!description.empty()) {
        cout << " - " << CYAN << description << RESET;
    }
    cout << endl;
}

void UISystem::showTradeInfo(const string& item, int price, int quantity, const string& customer) {
    cout << "  " << BRIGHT_WHITE << item << RESET;
    cout << " | Price: " << BRIGHT_YELLOW << formatCurrency(price) << RESET;
    cout << " | Qty: " << (quantity > 0 ? GREEN : RED) << quantity << RESET;

    if (!customer.empty()) {
        cout << " | Customer: " << BRIGHT_CYAN << customer << RESET;
    }
    cout << endl;
}

void UISystem::showSuccess(const string& message) {
    cout << BRIGHT_GREEN << "✓ " << message << RESET << endl;
}

void UISystem::showWarning(const string& message) {
    cout << BRIGHT_YELLOW << "⚠ " << message << RESET << endl;
}

void UISystem::showError(const string& message) {
    cout << BRIGHT_RED << "✗ " << message << RESET << endl;
}

void UISystem::showInfo(const string& message) {
    cout << BRIGHT_CYAN << "ℹ " << message << RESET << endl;
}

void UISystem::startSection(const string& title) {
    InputSystem::clearScreen();
    InputSystem::drawTitle(title);
    InputSystem::drawSeparator();
}

void UISystem::endSection() {
    InputSystem::drawSeparator();
    cout << STATUS_INFO << "Press any key to continue..." << RESET << endl;
    InputSystem::waitForAnyKey();
}

string UISystem::formatNumber(int number) {
    stringstream ss;
    ss << number;
    return ss.str();
}

string UISystem::formatCurrency(int amount) {
    stringstream ss;
    ss << "$" << amount;
    return ss.str();
}

string UISystem::getDangerLevelColor(int dangerLevel) {
    if (dangerLevel <= 2) return GREEN;
    if (dangerLevel <= 4) return YELLOW;
    return RED;
}

string UISystem::createProgressBar(int current, int max, int width, const string& color) {
    if (max == 0) return "[EMPTY]";

    int filled = (current * width) / max;
    int empty = width - filled;

    stringstream bar;
    bar << "[" << color;
    for (int i = 0; i < filled; ++i) bar << "█";
    bar << RESET;
    for (int i = 0; i < empty; ++i) bar << "░";
    bar << "]";

    return bar.str();
}

string UISystem::createHealthBar(int current, int max, int width) {
    string color;
    if (current > max * 0.6) color = GREEN;
    else if (current > max * 0.3) color = YELLOW;
    else color = RED;

    return createProgressBar(current, max, width, color);
}

string UISystem::createStaminaBar(int current, int max, int width) {
    string color;
    if (current > max * 0.5) color = BRIGHT_BLUE;
    else if (current > max * 0.2) color = BLUE;
    else color = BRIGHT_RED;

    return createProgressBar(current, max, width, color);
}