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

void InputSystem::initialize() {
    if (!isInitialized) {
        setNonBlockingInput();
        isInitialized = true;
    }
}

void InputSystem::cleanup() {
    if (isInitialized) {
        restoreTerminal();
        isInitialized = false;
    }
}

void InputSystem::setNonBlockingInput() {
    struct termios newTermios;
    tcgetattr(STDIN_FILENO, &originalTermios);
    newTermios = originalTermios;
    newTermios.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newTermios);
}

void InputSystem::restoreTerminal() {
    tcsetattr(STDIN_FILENO, TCSANOW, &originalTermios);
}

bool InputSystem::kbhit() {
    struct timeval tv = {0L, 0L};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    return select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0;
}

char InputSystem::getch() {
    char ch = 0;
    if (read(STDIN_FILENO, &ch, 1) != 1) {
        ch = 0;
    }
    return ch;
}

void InputSystem::clearScreen() {
    cout << "\033[2J\033[H";
}

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

void InputSystem::drawSeparator(int length) {
    if (length == -1) {
        length = getTerminalWidth();
    }
    cout << string(length, '=') << endl;
}

void InputSystem::drawTitle(const string &title) {
    int width = getTerminalWidth();
    int padding = (width - title.length()) / 2;
    if (padding < 0) padding = 0;

    cout << string(padding, ' ') << BOLD << title << RESET << endl;
}

int InputSystem::getTerminalWidth() {
    if (!widthCalculated) {
        int rows, cols;
        getTerminalSize(rows, cols);
        terminalWidth = cols;
        widthCalculated = true;
    }
    return terminalWidth;
}

void InputSystem::waitForAnyKey() {
    cout << YELLOW << "Press any key to continue..." << RESET << endl;
    while (!kbhit()) {
        sleepMs(100);
    }
    getch(); // Clear the key press
}

void InputSystem::sleepMs(int ms) {
    usleep(ms * 1000);
}