#include <iostream>
#include <thread>
#include <ncurses.h>
#include <mutex>
//#include "Snake.h"

using namespace std;

static mutex myMutex;

#define DIM_X 30
#define DIM_Y 15

#define SNAKE_START_X 10
#define SNAKE_START_Y 10

typedef enum{ DIR_0, DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT}dir_t;
const char b = '#';   //b for border
const char x = ' ';   //x for space
const char f = '*';   //f for fruit
const char s = 'o';   //s for space

class Fruit;


void demo(){
    std::cout << "Demo!" << std::endl;
}

struct coordinates{
    int x;
    int y;
};

class Snake{
    private:
        coordinates head;
        dir_t       snake_dir = DIR_0;
    public:
        Snake(){  setX(SNAKE_START_X); setY(SNAKE_START_Y);  };
        void draw(){ std::cout << "Sssss...." << std::endl; };

        void setX(const int x_coord){ head.x = x_coord; };
        void setY(const int y_coord){ head.y = y_coord; };
        void setDir(dir_t dir){ snake_dir = dir; };

        int getX(){ return head.x; };
        int getY(){ return head.y; };

        friend void drawMap(Snake snake, Fruit fruit);

};

class Fruit{
    private:
        coordinates fruitCoord = {20, 10};
        void setX(const int x_coord){ fruitCoord.x = x_coord; };
        void setY(const int y_coord){ fruitCoord.y = y_coord; };

    public:
        void randomizeFruit(){ 
                setX(rand() % DIM_X);
                setY(rand() % DIM_Y);
            };
        int getX(){ return fruitCoord.x; };
        int getY(){ return fruitCoord.y; };

        friend void drawMap(Snake snake, Fruit fruit);
};

class Model{
    private:
        int score;

};

class View{

    private:
        uint8_t dim_x;
        uint8_t dim_y;

    public:
        View(uint8_t x, uint8_t y){ dim_x = x;  dim_y = y;};
        int getDimX(){ return dim_x; };
        int getDimY(){ return dim_y; };

        void drawMap(Snake snake, Fruit fruit){

            fruit.randomizeFruit();

            for(int i = 0; i < dim_x; i++)
                std::cout << b;
            
            std::cout << "\n\r";

            for(int i = 0; i < dim_y; i++){

                for(int j = 0; j < dim_x; j++){
                    if(j == 0 || j == dim_x - 1) {
                        std::cout << b;
                        if(j == dim_x - 1)  std::cout << "\n\r";
                    }
                    else{
                        if( i == fruit.getY() && j == fruit.getX()){  
                            std::cout << f;  }

                        else if(i == snake.getY() && j == snake.getX()){
                            std::cout << s;
                        }
                        else    
                            std::cout << x;
                    }

                }
            }

            for(int i = 0; i < dim_x; i++)
                std::cout << b;

            std::cout << "\n\r";
            std::cout << "\n\r";
        }
};

class Controller{

    public:
        void interpretDirection(){  //might have to do getchar in a different Thread
            
            char dirChar = getch();
            
            switch(dirChar){
                case 'a':
                std::cout << "dirChar: " << dirChar << "\n\r";
                    break;

                case 'd':
                std::cout << "dirChar: " << dirChar << "\n\r";
                    break;

                case 'w':
                std::cout << "dirChar: " << dirChar << "\n\r";
                    break;

                case 's':
                std::cout << "dirChar: " << dirChar << "\n\r";
                    break;

                default:
                break;
            }
        };
};


void mainThread(){
    Snake snake;
    //snake.draw();

    Fruit fruit;

    View gameView(DIM_X,DIM_Y);

    //std::lock_guard<std::mutex> lock(myMutex);
    gameView.drawMap(snake, fruit);

}


void interpretDir(){
    Controller control;
    initscr();
    nodelay(stdscr, true);
    noecho();
    raw();
    
    while(1)
        control.interpretDirection();
    
    endwin();
}

//check this out!
//https://betterprogramming.pub/writing-framework-for-inter-thread-message-passing-in-c-256b5308a471

int main(){

while(1){
    std::thread thread1(interpretDir);
    std::thread thread2(mainThread);

    //synchronize threads:
    thread1.join();                // pauses until first finishes
    thread2.join();               // pauses until second finishes
}


}