// InputSystem.cpp - Unified input handling system implementation
#include "InputSystem.h"
#include <iostream>
#include <unistd.h>
#include <sys/ioctl.h>
#include <cstdlib>
#include <cstdio>

using namespace std;

// Static member initialization
struct termios InputSystem::originalTermios;
bool InputSystem::isInitialized = false;
int InputSystem::terminalWidth = 80;
bool InputSystem::widthCalculated = false;

// Initializes the input system for non-blocking input
// What it does: Sets up terminal for non-blocking input mode
// Inputs: None
// Outputs: None
void InputSystem::initialize() {
    if (!isInitialized) {
        setNonBlockingInput();
        isInitialized = true;
    }
}

// Cleans up the input system and restores terminal settings
// What it does: Restores original terminal settings to prevent terminal corruption
// Inputs: None
// Outputs: None
void InputSystem::cleanup() {
    if (isInitialized) {
        restoreTerminal();
        isInitialized = false;
    }
}

// Configures terminal for non-blocking input mode
// What it does: Disables canonical mode and echo to allow immediate key detection
// Inputs: None
// Outputs: None
void InputSystem::setNonBlockingInput() {
    struct termios newTermios;
    tcgetattr(STDIN_FILENO, &originalTermios);
    newTermios = originalTermios;
    newTermios.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newTermios);
}

// Restores original terminal settings
// What it does: Returns terminal to its original state after non-blocking mode
// Inputs: None
// Outputs: None
void InputSystem::restoreTerminal() {
    tcsetattr(STDIN_FILENO, TCSANOW, &originalTermios);
}

// Checks if a key has been pressed (non-blocking)
// What it does: Returns true if a key is available to read, false otherwise
// Inputs: None
// Outputs: bool - True if key pressed, false otherwise
bool InputSystem::kbhit() {
    struct timeval tv = {0L, 0L};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    return select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0;
}

// Gets a single character from input (non-blocking)
// What it does: Reads one character from stdin without waiting
// Inputs: None
// Outputs: char - The character read, or 0 if no character available
char InputSystem::getch() {
    char ch = 0;
    if (read(STDIN_FILENO, &ch, 1) != 1) {
        ch = 0;
    }
    return ch;
}

// Clears the terminal screen
// What it does: Uses ANSI escape codes to clear screen and move cursor to top
// Inputs: None
// Outputs: None
void InputSystem::clearScreen() {
    cout << "\033[2J\033[H";
}

// Gets the current terminal dimensions
// What it does: Queries terminal for rows and columns using ioctl
// Inputs: rows - Reference to store number of rows
//         cols - Reference to store number of columns
// Outputs: None (updates parameters by reference)
void InputSystem::getTerminalSize(int &rows, int &cols) {
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) != -1) {
        rows = ws.ws_row;
        cols = ws.ws_col;
    } else {
        rows = 24;
        cols = 80;
    }
}

// Draws a horizontal separator line
// What it does: Creates a line of '=' characters, defaults to terminal width
// Inputs: length - Length of separator, -1 for full terminal width
// Outputs: None
void InputSystem::drawSeparator(int length) {
    if (length == -1) {
        length = getTerminalWidth();
    }
    cout << string(length, '=') << endl;
}

// Draws a centered title with bold formatting
// What it does: Centers the title text and applies bold formatting
// Inputs: title - The title text to display
// Outputs: None
void InputSystem::drawTitle(const string &title) {
    int width = getTerminalWidth();
    int padding = (width - title.length()) / 2;
    if (padding < 0) padding = 0;

    cout << string(padding, ' ') << BOLD << title << RESET << endl;
}

// Gets the current terminal width (cached)
// What it does: Returns terminal width, calculates once and caches result
// Inputs: None
// Outputs: int - Terminal width in characters
int InputSystem::getTerminalWidth() {
    if (!widthCalculated) {
        int rows, cols;
        getTerminalSize(rows, cols);
        terminalWidth = cols;
        widthCalculated = true;
    }
    return terminalWidth;
}

// Waits for any key press to continue
// What it does: Displays prompt and waits until any key is pressed
// Inputs: None
// Outputs: None
void InputSystem::waitForAnyKey() {
    cout << YELLOW << "Press any key to continue..." << RESET << endl;
    while (!kbhit()) {
        sleepMs(100);
    }
    getch(); // Clear the key press
}

// Sleeps for specified milliseconds
// What it does: Pauses execution for the given time using usleep
// Inputs: ms - Number of milliseconds to sleep
// Outputs: None
void InputSystem::sleepMs(int ms) {
    usleep(ms * 1000);
}