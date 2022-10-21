#include <iostream>
#include <thread>
#include <ncurses.h>
#include <mutex>
#include <chrono>
#include <vector>
#include <stdio.h>

using namespace std;

#define DIM_X 30
#define DIM_Y 15

#define SNAKE_START_X 10
#define SNAKE_START_Y 10

typedef enum{ DIR_0, DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT}dir_t;
const char b = '#';   //b for border
const char x = ' ';   //x for space
const char f = '*';   //f for fruit
const char s = 'o';   //s for snake

bool gameEnd = false;

class Fruit;

void demo(){
    std::cout << "Demo!" << std::endl;
}

struct coordinates{
    int x;
    int y;
};

static mutex myMutex;
static char buttonPressed = 'n'; //none

class Snake{
    private:
        coordinates head;
        coordinates tail[25];
        volatile int length = 0;
        dir_t snake_dir = DIR_0;
    public:
        Snake(){  setX(SNAKE_START_X); setY(SNAKE_START_Y);  }

        void setX(const int x_coord){ head.x = x_coord; };
        void setY(const int y_coord){ head.y = y_coord; };
        void setDir(dir_t dir){ snake_dir = dir; };

        int getX(){ return head.x; };
        int getY(){ return head.y; };

        int getTailX(int i){ return tail[i].x; };
        int getTailY(int i){ return tail[i].y; };

        int searchTailX(int tailX){
            for(int i = 0; i < length; i++){
                if(tail[i].x == tailX){
                    return tailX;
                }
            }
            return -1;
        };

        int searchTailY(int tailY){
            for(int i = 0; i < length; i++){
                if(tail[i].y == tailY){
                    return tailY;
                }
            }
            return -1;
        };

        int searchTailByCoordinate(coordinates coord){
            for(int i = 0; i < length; i++){
                if(tail[i].y == coord.y && tail[i].x == coord.x){
                    return 1;
                }
        }
            return -1;
        };

        int getLength(){ return length; };
        void setLength(int len){ length = len; };

        void updateBody(){

            for(int i=length-1; i>=0; i--){
                tail[i+1].x = tail[i].x;
                tail[i+1].y = tail[i].y;
            }

            tail[0].x = head.x;
            tail[0].y = head.y;

            if(tail[0].x <= 0 || tail[0].x >= DIM_X - 1 || tail[0].y < 0 || tail[0].y > DIM_Y){  //the snake came in contact with the borders
                gameEnd = true;
                cout << " game end triggered..." << "\n\r";
            }

            //cout << "head at: " << head.x << " , " << head.y << "\n\r";
            //cout << "tail at: " << tail[0].x << " , " << tail[0].y << " | " << tail[1].x << " , " << tail[1].y << " | " << tail[2].x << " , " << tail[2].y << "\n\r";
        }

        void resetSnake(){

            length = 0;

            for(int i=length-1; i>=0; i--){
                tail[i].x = SNAKE_START_X;
                tail[i].y = SNAKE_START_Y;
            }

            setX(SNAKE_START_X); setY(SNAKE_START_Y);
        }

       void interpretDirection(){

            myMutex.lock();
            char dirChar = buttonPressed;
            myMutex.unlock();

            switch(dirChar){
                case 'a':
                    setX(--head.x);
                    break;

                case 'd':
                    setX(++head.x);
                    break;

                case 'w':
                    
                    if(gameEnd == true){
                        gameEnd = false;
                        buttonPressed = 'n';
                    }
                    else setY(--head.y);

                    break;

                case 's':
                    setY(++head.y);
                    break;

                default:
                break;
            }

        }

        friend void drawMap(Snake snake, Fruit fruit);
        friend class Model;
};

class Fruit{
    private:
        coordinates fruitCoord = {20, 10};
        void setX(const int x_coord){ fruitCoord.x = x_coord; };
        void setY(const int y_coord){ fruitCoord.y = y_coord; };

    public:
        void randomizeFruit(){ 
                int tempX = rand() % DIM_X;
                int tempY = rand() % DIM_Y;

                if(tempX == 0)  tempX = 1;
                if(tempY == 0)  tempY = 1;

                setX(tempX);
                setY(tempY);
            };

        void resetFruit(){
            fruitCoord = {20, 10};
        }

        int getX(){ return fruitCoord.x; };
        int getY(){ return fruitCoord.y; };

        friend void drawMap(Snake snake, Fruit fruit);
        friend class Model;
};

class Model{
    private:

    public:
        int isFruitEaten(Snake &snake, Fruit &fruit){
            if(snake.head.x == fruit.fruitCoord.x && snake.head.y == fruit.fruitCoord.y){
                int len = snake.getLength();
                fruit.randomizeFruit();
                snake.setLength(++len);
                return true;
            }

             return false;
        }

        void checkGameState(Snake &snake, Fruit &fruit){
            if(gameEnd){
                snake.resetSnake();
                fruit.resetFruit();
            }
        }

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

            if(gameEnd)
            {
                std::cout << "GAME OVER!!!\n\r";
                std::cout << "press 'w' to continue...\n\r";
                return;
            }

            for(int i = 0; i < dim_x; i++)
                std::cout << b;
            
            std::cout << "\n\r";

            for(int i = 0; i < dim_y; i++){

                for(int j = 0; j < dim_x; j++){

                    coordinates coord = {j,i};

                    if(j == 0 || j == dim_x - 1) {
                        std::cout << b;
                        if(j == dim_x - 1)  std::cout << "\n\r";
                    }
                    else{
                        if( i == fruit.getY() && j == fruit.getX()){  
                            std::cout << f;  
                            }

                        else if(i == snake.getY() && j == snake.getX()){
                            std::cout << 'O';
                        }

                        else if(snake.searchTailByCoordinate(coord) == 1)
                            std::cout << 'o';
                            
                        else   
                            std::cout << x;
                    }

                }
            }

            for(int i = 0; i < dim_x; i++)
                std::cout << b;

            std::cout << "\n\r";
            std::cout << "Len: " << snake.getLength() << "\n\r";
            std::cout << "Fruit at: (" << fruit.getX() << ","<< fruit.getY() << ")" << "\n\r";
            std::cout << "\n\r";
        }
};

class Controller{

    public:
        void getDirectionChar(){  //might have to do getchar in a different Thread
            
            char dirChar = getch();

            if(dirChar == 'a' || dirChar == 'w' || dirChar == 's' ||dirChar == 'd'){
                std::lock_guard<std::mutex> lock(myMutex);
                buttonPressed = dirChar;
            }

        };
};


void mainThread(){
    
    Snake snake;
    Fruit fruit;
    View gameView(DIM_X,DIM_Y);
    Model model;

    while(1){
        model.checkGameState(snake, fruit);
        snake.interpretDirection();
        gameView.drawMap(snake, fruit);
        model.isFruitEaten(snake, fruit);
        snake.updateBody();
        std::this_thread::sleep_for(chrono::milliseconds(200));
        system("clear");
    }

}


void getDir(){
    Controller control;
    initscr();
    nodelay(stdscr, true);
    noecho();
    raw();
    
    while(1)
        control.getDirectionChar();
    
    endwin();
}

//check this out!
//https://betterprogramming.pub/writing-framework-for-inter-thread-message-passing-in-c-256b5308a471

int main(){

    std::thread thread1(getDir);
    std::thread thread2(mainThread);

    //synchronize threads:
    thread1.join();                // pauses until first finishes
    thread2.join();               // pauses until second finishes


}