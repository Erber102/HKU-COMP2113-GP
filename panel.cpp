#include<string>
#include<vector>
#include<iostream>
#include<termio.h>
#include<unistd.h>
//use ANSI to draw the UI
class textGame {
private: 
    int width,height;
    std::vector<std::string> screen;
    std::vector<std::vector<bool>> dirty;
    struct termios originalTermios;
public:
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
    ~textGame() {
        //recover terminal
        tcsetattr(STDIN_FILENO,TCSANOW,&originalTermios);
        //show cursor
        std::cout<<"\033[?25l";
        //clear
        std::cout<<"\033[2J\033[H";
        std::cout.flush();
    }
    void clear() {
        for(auto &row:screen) {
            std::fill(row.begin(),row.end(),' ');
        }
        for(auto &row:dirty) {
            std::fill(row.begin(),row.end(),true);
        }
    }
    //set char to buffer
    void setCell(int x,int y,char c) {
        if(x>=0&&x<width&&y>=0&&y<height) {
            if(screen[y][x]!=c) {
                screen[y][x]=c;
                dirty[y][x]=true;
            }
        }
    }
    //get cell
    char getCell(int x,int y) {
        if(x>=0&&x<width&&y>=0&&y<height) {
            return screen[y][x];
        }
        return ' ';
    }
    // put the text to buffer
    void drawText(int x,int y,std::string txt) {
        int sz=txt.length();
        for(int i=0;i<sz&&x+i<width;i++) {
            setCell(x+i,y,txt[i]);
        }
    }
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
    //non-blocking input check
    bool kbhit() {
        struct timeval tv={0,0};
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO,&fds);
        return select(STDIN_FILENO+1,&fds,NULL,NULL,&tv)==1;
    }
    //get input
    char getInput() {
        if(kbhit()) {
            char ch;
            read(STDIN_FILENO,&ch,1);
            return ch;
        }
        return 0;
    }
    //render the changed parts
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
    int getWidth() {
        return width;
    }
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
    fight(int w,int h):pn(w,h),playerX(w/2),playerY(h/2),oldX(playerX),oldY(playerY),run(true) {}
    void init() {
        pn.drawBorder();
        pn.drawText(2,1,"used WASD to move");
        pn.initRender();
    }
    //handle the input
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
    //update the position after every movement
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
    bool isRun() {
        return run;
    }
    void clean() {
        pn.clear();
        pn.initRender();
    }
};
void game() {
    fight fgt(40,20);//size of the UI
    fgt.init();
    while(fgt.isRun()) {
        fgt.update();
    }
    fgt.clean();
}