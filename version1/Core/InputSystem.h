// InputSystem.h - Unified input handling system
#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

#include <string>
#include <termios.h>

// ANSI escape sequences for colors
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"
#define BOLD "\033[1m"

class InputSystem {
private:
    static struct termios originalTermios;
    static bool isInitialized;
    static int terminalWidth;
    static bool widthCalculated;

public:
    // Input methods
    static void initialize();
    static void cleanup();
    static void setNonBlockingInput();
    static void restoreTerminal();
    static bool kbhit();
    static char getch();

    // Terminal display methods
    static void clearScreen();
    static void getTerminalSize(int &rows, int &cols);
    static void drawSeparator(int length = -1);
    static void drawTitle(const std::string &title);
    static int getTerminalWidth();

    // Utility methods
    static void waitForAnyKey();
    static void sleepMs(int ms);
};

#endif