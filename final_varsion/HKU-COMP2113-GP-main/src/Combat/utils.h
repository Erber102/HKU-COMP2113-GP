#ifndef UTILS_H
#define UTILS_H
#include<string>
//use ANSI escape sequences to display colors
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"
#define BOLD "\033[1m"

enum Note {DO=1,RE,MI,FA,SOL,LA,SI};
using namespace std;
class Terminal{
private:
    static int terminalWidth;
    static bool widthCalculated;
    static struct termios originalTermios;
public:
    static void setNonBlockingInput();
    static bool kbhit();
    static char getch();
    static void clearScreen();
    static void getTerminalSize(int &rows,int &cols);
    static void drawSeparator(int length=-1);
    static void drawTitle(string title);
    static int getTerminalWidth();
    static void restoreTerminal();
};

namespace Utils{
    string noteToString(Note note);
    void waitForAnyKey();
    void sleepMs(int ms);
    string formatHealthBar(int current,int max,int width=20);
    string formatResonanceBar(int current,int width=15);
    int randomInt(int leftBound,int rightBound);
}

#endif