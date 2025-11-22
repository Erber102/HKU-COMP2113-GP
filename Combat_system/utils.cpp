#include "utils.h"
#include<iostream>
#include<termio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/ioctl.h>
#include<chrono>
#include<thread>
#include<algorithm>
#include<cstdlib>
#include<ctime>
using namespace std;
int Terminal::terminalWidth=80;
bool Terminal::widthCalculated=0;
struct termios Terminal::originalTermios;
//enable nonblocking input
void Terminal::setNonBlockingInput(){
    struct termios newTerminal;
    tcgetattr(STDIN_FILENO,&originalTermios);
    //hide the cursor
    cout<<"\x1b[?25l"<<endl;
    newTerminal=originalTermios;
    newTerminal.c_lflag&=~(ICANON|ECHO);
    tcsetattr(STDIN_FILENO,TCSANOW,&newTerminal);
    fcntl(STDIN_FILENO,F_SETFL,O_NONBLOCK);
}
//reset terminal settings
void Terminal::restoreTerminal(){
    //enabling the cursor
    cout<<"\x1b[?25h"<<endl;
    tcsetattr(STDIN_FILENO,TCSANOW,&originalTermios);
    fcntl(STDIN_FILENO,F_SETFL,0);
}
//check keyboard input
bool Terminal::kbhit(){
    struct timeval tv={0,0};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO,&fds);
    return select(STDIN_FILENO+1,&fds,NULL,NULL,&tv)>0;
}
//get ketboard input
char Terminal::getch(){
    char c=0;
    read(STDIN_FILENO,&c,1);
    return c;
}

void Terminal::clearScreen(){
    cout<<"\033[2J\033[1;1H";
}

void Terminal::getTerminalSize(int &rows,int &cols){
    struct winsize w;
    ioctl(STDOUT_FILENO,TIOCGWINSZ, &w);
    rows=w.ws_row;
    cols=w.ws_col;
    terminalWidth=cols;
    widthCalculated=1;
}

int Terminal::getTerminalWidth(){
    if(!widthCalculated){
        int rows,cols;
        getTerminalSize(rows,cols);
    }
    return terminalWidth;
}
//draw separating line
void Terminal::drawSeparator(int length){
    if(length==-1){
        length=getTerminalWidth()-2;
        length=max(length,20);
    }
    cout<<CYAN;
    for(int i=0;i<length;i++)cout<<"─";
    cout<<RESET<<endl;
}

void Terminal::drawTitle(string title){
    int width=getTerminalWidth();
    int padding=(width-title.length()-4)/2;
    padding=max(padding,0);
    cout<<MAGENTA<<BOLD;
    for(int i=0;i<padding;i++)cout<<" ";
    cout<<title<<RESET<<endl;
}
//notes to string
string Utils::noteToString(Note note){
    switch(note){
        case DO:return "Do";
        case RE:return "Re";
        case MI:return "Mi";
        case FA:return "Fa";
        case SOL:return "Sol";
        case LA:return "La";
        case SI:return "Si";
        default:return "?";
    }
}

void Utils::waitForAnyKey(){
    cout<<YELLOW<<"Press any key to continue"<<RESET<<endl;
    while(!Terminal::kbhit()){
        sleepMs(100);
    }
    Terminal::getch();
}

void Utils::sleepMs(int ms){
    this_thread::sleep_for(chrono::milliseconds(ms));
}

string Utils::formatHealthBar(int current,int max,int width){
    double percent=(double)current/max;
    int filled=width*percent;
    string bar="[";
    for(int i=0;i<width;i++){
        if(i<filled){
            if(percent>0.7)bar+=GREEN;
            else if(percent>0.3)bar+=YELLOW;
            else bar+=RED;
            bar+="█";
        }
        else bar+=" ";
    }
    bar+=RESET;
    bar+="] "+to_string(current)+"/"+to_string(max)+" HP";
    return bar;
}

string Utils::formatResonanceBar(int current,int width){
    double percent=(double)current/100;
    int filled=width*percent;
    string bar="[";
    for(int i=0;i<width;i++){
        if(i<filled){
            bar+=BLUE+string("█");
        }
        else bar+=" ";
    }
    bar+=RESET;
    bar+="]"+to_string(current)+"/100";
    return bar;
}

int Utils::randomInt(int leftBound,int rightBound){
    return rand()%(rightBound-leftBound+1)+leftBound;
}