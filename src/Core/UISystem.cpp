// UISystem.cpp - Unified UI system implementation
#include "UISystem.h"
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

// Displays a menu with title and options
// What it does: Clears screen, shows centered title, and lists numbered options
// Inputs: title - Menu title text
//         options - List of menu option strings
// Outputs: None
void UISystem::showMenu(const string& title, const vector<string>& options) {
    InputSystem::clearScreen();
    InputSystem::drawTitle(title);
    InputSystem::drawSeparator();

    for (size_t i = 0; i < options.size(); ++i) {
        cout << " " << BRIGHT_CYAN << "[" << (i + 1) << "]" << RESET << " " << options[i] << endl;
    }

    InputSystem::drawSeparator();
}

// Gets user menu selection from displayed options
// What it does: Shows menu and waits for valid numeric input
// Inputs: title - Menu title text
//         options - List of menu option strings
// Outputs: int - Selected option number (1-based)
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

// Gets a numeric choice within specified range
// What it does: Waits for valid numeric input between min and max
// Inputs: min - Minimum allowed value
//         max - Maximum allowed value
//         prompt - Optional prompt text
// Outputs: int - Selected number, or -1 if 'q' pressed
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

// Displays player status with health and stamina bars
// What it does: Shows health, stamina, and money with colored progress bars
// Inputs: hp - Current health points
//         maxHp - Maximum health points
//         stamina - Current stamina points
//         maxStamina - Maximum stamina points
//         money - Current money amount
// Outputs: None
void UISystem::showStatusBar(int hp, int maxHp, int stamina, int maxStamina, int money) {
    cout << BOLD << "=== PLAYER STATUS ===" << RESET << endl;

    string healthColor = (hp > maxHp * 0.6) ? GREEN : (hp > maxHp * 0.3) ? YELLOW : RED;
    string staminaColor = (stamina > maxStamina * 0.5) ? GREEN : (stamina > maxStamina * 0.2) ? YELLOW : RED;

    cout << "Health:   " << healthColor << createHealthBar(hp, maxHp) << " " << hp << "/" << maxHp << " HP" << RESET << endl;
    cout << "Stamina:  " << staminaColor << createStaminaBar(stamina, maxStamina) << " " << stamina << "/" << maxStamina << RESET << endl;
    cout << "Money:    " << BRIGHT_YELLOW << formatCurrency(money) << RESET << endl;

    InputSystem::drawSeparator();
}

// Displays inventory section header
// What it does: Shows formatted inventory title
// Inputs: None
// Outputs: None
void UISystem::showInventoryHeader() {
    cout << BOLD << BRIGHT_CYAN << "=== INVENTORY ===" << RESET << endl;
}

// Displays a single inventory item entry
// What it does: Shows item name, quantity, and optional description with color coding
// Inputs: itemName - Name of the item
//         quantity - Number of items
//         description - Optional description text
// Outputs: None
void UISystem::showItemEntry(const string& itemName, int quantity, const string& description) {
    string quantityColor = (quantity > 0) ? GREEN : RED;

    cout << "  " << BRIGHT_WHITE << itemName << RESET;
    cout << " x" << quantityColor << quantity << RESET;

    if (!description.empty()) {
        cout << " - " << CYAN << description << RESET;
    }
    cout << endl;
}

// Displays trade information for an item
// What it does: Shows item details with price, quantity, and customer info
// Inputs: item - Item name
//         price - Item price
//         quantity - Item quantity
//         customer - Customer name (optional)
// Outputs: None
void UISystem::showTradeInfo(const string& item, int price, int quantity, const string& customer) {
    cout << "  " << BRIGHT_WHITE << item << RESET;
    cout << " | Price: " << BRIGHT_YELLOW << formatCurrency(price) << RESET;
    cout << " | Qty: " << (quantity > 0 ? GREEN : RED) << quantity << RESET;

    if (!customer.empty()) {
        cout << " | Customer: " << BRIGHT_CYAN << customer << RESET;
    }
    cout << endl;
}

// Displays a success message with checkmark
// What it does: Shows green success message with checkmark prefix
// Inputs: message - Success message text
// Outputs: None
void UISystem::showSuccess(const string& message) {
    cout << BRIGHT_GREEN << "✓ " << message << RESET << endl;
}

// Displays a warning message with warning symbol
// What it does: Shows yellow warning message with warning symbol prefix
// Inputs: message - Warning message text
// Outputs: None
void UISystem::showWarning(const string& message) {
    cout << BRIGHT_YELLOW << "⚠ " << message << RESET << endl;
}

// Displays an error message with X symbol
// What it does: Shows red error message with X symbol prefix
// Inputs: message - Error message text
// Outputs: None
void UISystem::showError(const string& message) {
    cout << BRIGHT_RED << "✗ " << message << RESET << endl;
}

// Displays an info message with info symbol
// What it does: Shows cyan info message with info symbol prefix
// Inputs: message - Info message text
// Outputs: None
void UISystem::showInfo(const string& message) {
    cout << BRIGHT_CYAN << "ℹ " << message << RESET << endl;
}

// Starts a new UI section with title
// What it does: Clears screen and displays section title with separator
// Inputs: title - Section title text
// Outputs: None
void UISystem::startSection(const string& title) {
    InputSystem::clearScreen();
    InputSystem::drawTitle(title);
    InputSystem::drawSeparator();
}

// Ends a UI section with separator and wait prompt
// What it does: Draws separator and waits for user to press any key
// Inputs: None
// Outputs: None
void UISystem::endSection() {
    InputSystem::drawSeparator();
    cout << STATUS_INFO << "Press any key to continue..." << RESET << endl;
    InputSystem::waitForAnyKey();
}

// Formats a number as string
// What it does: Converts integer to string using stringstream
// Inputs: number - Integer to format
// Outputs: string - Formatted number string
string UISystem::formatNumber(int number) {
    stringstream ss;
    ss << number;
    return ss.str();
}

// Formats currency amount with dollar sign
// What it does: Converts integer to currency string with $ prefix
// Inputs: amount - Currency amount
// Outputs: string - Formatted currency string
string UISystem::formatCurrency(int amount) {
    stringstream ss;
    ss << "$" << amount;
    return ss.str();
}

// Gets color code for danger level
// What it does: Returns ANSI color code based on danger level (green/yellow/red)
// Inputs: dangerLevel - Danger level from 1-5
// Outputs: string - ANSI color code string
string UISystem::getDangerLevelColor(int dangerLevel) {
    if (dangerLevel <= 2) return GREEN;
    if (dangerLevel <= 4) return YELLOW;
    return RED;
}

// Creates a visual progress bar
// What it does: Generates a text-based progress bar with filled and empty sections
// Inputs: current - Current value
//         max - Maximum value
//         width - Width of the progress bar
//         color - ANSI color code for filled section
// Outputs: string - Formatted progress bar string
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

// Creates a health progress bar with color coding
// What it does: Generates health bar with color based on health percentage
// Inputs: current - Current health points
//         max - Maximum health points
//         width - Width of the progress bar
// Outputs: string - Formatted health bar string
string UISystem::createHealthBar(int current, int max, int width) {
    string color;
    if (current > max * 0.6) color = GREEN;
    else if (current > max * 0.3) color = YELLOW;
    else color = RED;

    return createProgressBar(current, max, width, color);
}

// Creates a stamina progress bar with color coding
// What it does: Generates stamina bar with color based on stamina percentage
// Inputs: current - Current stamina points
//         max - Maximum stamina points
//         width - Width of the progress bar
// Outputs: string - Formatted stamina bar string
string UISystem::createStaminaBar(int current, int max, int width) {
    string color;
    if (current > max * 0.5) color = BRIGHT_BLUE;
    else if (current > max * 0.2) color = BLUE;
    else color = BRIGHT_RED;

    return createProgressBar(current, max, width, color);
}