#include<string>
#include<vector>
#include<iostream>
#include<termios.h>
#include<unistd.h>
//use ANSI to draw the UI
class textGame {
private: 
    int width,height;
    std::vector<std::string> screen;
    std::vector<std::vector<bool>> dirty;
    struct termios originalTermios;
public:
    // Initializes terminal-based text game with screen buffer and input handling
    // Input: w - width of the game screen, h - height of the game screen
    // Output: None
    textGame(int w,int h):width(w), height(h) {
        //initialize screen buffer
        screen.resize(height,std::string(width,' '));
        dirty.resize(height,std::vector<bool>(width,false));
        //save the original terminal setting
        tcgetattr(STDIN_FILENO,&originalTermios);
        //allow non-blocking input
        struct termios newTermios=originalTermios;
        newTermios.c_lflag&=~(ICANON|ECHO);
        tcsetattr(STDIN_FILENO,TCSANOW,&newTermios);
        //hide cursor
        std::cout<<"\033[?25l";
        //initial clear
        std::cout<<"\033[2J\033[H";
        std::cout.flush();
    }

    // Restores terminal settings and cleans up display when object is destroyed
    // Input: None
    // Output: None
    ~textGame() {
        //recover terminal
        tcsetattr(STDIN_FILENO,TCSANOW,&originalTermios);
        //show cursor
        std::cout<<"\033[?25l";
        //clear
        std::cout<<"\033[2J\033[H";
        std::cout.flush();
    }

    // Clears the entire screen buffer and marks all cells as dirty for redraw
    // Input: None
    // Output: None
    void clear() {
        for(auto &row:screen) {
            std::fill(row.begin(),row.end(),' ');
        }
        for(auto &row:dirty) {
            std::fill(row.begin(),row.end(),true);
        }
    }

    // Sets a character at specified coordinates in the screen buffer
    // Input: x - horizontal coordinate, y - vertical coordinate, c - character to set
    // Output: None
    void setCell(int x,int y,char c) {
        if(x>=0&&x<width&&y>=0&&y<height) {
            if(screen[y][x]!=c) {
                screen[y][x]=c;
                dirty[y][x]=true;
            }
        }
    }

    // Retrieves the character at specified coordinates from the screen buffer
    // Input: x - horizontal coordinate, y - vertical coordinate
    // Output: character at the specified position, or space if out of bounds
    char getCell(int x,int y) {
        if(x>=0&&x<width&&y>=0&&y<height) {
            return screen[y][x];
        }
        return ' ';
    }

    // Draws text string starting at specified coordinates in the screen buffer
    // Input: x - starting horizontal coordinate, y - vertical coordinate, txt - text string to draw
    // Output: None
    void drawText(int x,int y,std::string txt) {
        int sz=txt.length();
        for(int i=0;i<sz&&x+i<width;i++) {
            setCell(x+i,y,txt[i]);
        }
    }

    // Draws a border around the edges of the screen using ASCII characters
    // Input: None
    // Output: None
    void drawBorder() {
        for(int i=0;i<width;i++)setCell(i,0,'-');//upper
        for(int i=0;i<width;i++)setCell(i,height-1,'-');//lower
        for(int i=0;i<height;i++)setCell(0,i,'|');//left
        for(int i=0;i<height;i++)setCell(width-1,i,'|');//right
        setCell(0,0,'+');
        setCell(width-1,0,'+');
        setCell(0,height-1,'+');
        setCell(width-1,height-1,'+');
    }

    // Checks if a key has been pressed without blocking program execution
    // Input: None
    // Output: boolean indicating whether input is available (true) or not (false)
    bool kbhit() {
        struct timeval tv={0,0};
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO,&fds);
        return select(STDIN_FILENO+1,&fds,NULL,NULL,&tv)==1;
    }

    // Reads a single character from standard input if available
    // Input: None
    // Output: character that was pressed, or 0 if no input available
    char getInput() {
        if(kbhit()) {
            char ch;
            read(STDIN_FILENO,&ch,1);
            return ch;
        }
        return 0;
    }

    // Renders only the changed portions of the screen buffer to the terminal
    // Input: None
    // Output: None
    void render() {
        std::string output;
        output.reserve(width*height*10);
        for(int y=0;y<height;y++) {
            for(int x=0;x<width;x++)
            {
                if(!dirty[y][x])continue;
                output+="\033[";
                output+=std::to_string(y+1);
                output+=";";
                output+=std::to_string(x+1);
                output+="H";
                output+=screen[y][x];
                dirty[y][x]=false;
            }
        }
        std::cout<<output;
        std::cout.flush();
    }

    // Performs initial full render of the entire screen buffer to terminal
    // Input: None
    // Output: None
    void initRender() {
        std::cout<<"\033[H";
        for(auto row:screen) {
            std::cout<<row<<std::endl;
        }
        for(auto &row:dirty) {
            std::fill(row.begin(),row.end(),false);
        }
        std::cout.flush();
    }

    // Returns the current width of the game screen
    // Input: None
    // Output: integer representing screen width
    int getWidth() {
        return width;
    }

    // Returns the current height of the game screen
    // Input: None
    // Output: integer representing screen height
    int getHeight() {
        return height;
    }
};
class fight {
private:
    textGame pn;//operation panel
    int playerX,playerY;
    int oldX,oldY;
    bool run;
public: 
    // Initializes fight game with player character and game state
    // Input: w - width of game area, h - height of game area
    // Output: None
    fight(int w,int h):pn(w,h),playerX(w/2),playerY(h/2),oldX(playerX),oldY(playerY),run(true) {}

    // Sets up initial game display with border and instructions
    // Input: None
    // Output: None
    void init() {
        pn.drawBorder();
        pn.drawText(2,1,"used WASD to move");
        pn.initRender();
    }

    // Processes keyboard input to control player movement and game state
    // Input: input - character representing the key pressed by user
    // Output: None
    void dealInput(char input) {
        switch (input) {
            case 'w':case 'W':
                if(playerY>1)playerY--;
                break;
            case 's':case 'S':
                if(playerY<pn.getHeight()-2)playerY++;
                break;
            case 'a':case 'A':
                if(playerX>1)playerX--;
                break;
            case 'd':case 'D':
                if(playerY<pn.getWidth()-2)playerX++;
                break;
            case 'q':case 'Q':
                run=false;
                break;
        }
    }

    // Updates game state including player position and handles input
    // Input: None
    // Output: None
    void update() {
        if(oldX!=playerX||oldY!=playerY) {
            if(pn.getCell(oldX,oldY)=='@') {
                pn.setCell(oldX,oldY,' ');
            }
        }
        pn.setCell(playerX,playerY,'@');
        char input=pn.getInput();
        oldX=playerX;oldY=playerY;
        dealInput(input);
        pn.render();
    }

    // Checks if the game is still running
    // Input: None
    // Output: boolean indicating whether game should continue (true) or exit (false)
    bool isRun() {
        return run;
    }

    // Cleans up the game display by clearing the screen
    // Input: None
    // Output: None
    void clean() {
        pn.clear();
        pn.initRender();
    }
};

// Main game function that initializes and runs the fight game loop
// Input: None
// Output: None
void game() {
    fight fgt(40,20);//size of the UI
    fgt.init();
    while(fgt.isRun()) {
        fgt.update();
    }
    fgt.clean();
}
