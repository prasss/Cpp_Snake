#include "stdint.h"

struct coordinates{
    uint8_t x;
    uint8_t y;
};
void demo();

class Snake{
    private:
        //coordinates head;
    public:
        Snake();
        void draw();
};