#pragma once
#include <stdint.h>

#define DOWN 0
#define LEFT 1
#define UP 2
#define RIGHT 3

enum Maps
{
    POSITION_MAP,
    LEFT_MAP,
    UP_MAP
};



//Direction dir_arr[4] = {{1, 0}, {0, -1}, {-1, 0}, {0, 1}};

struct Direction
{
    int8_t y: 4;
    int8_t x: 4;

    Direction(int8_t y_val = 1, int8_t x_val = 0) : y(y_val), x(x_val) {}
};

extern Direction dir_arr[4];

void fix_position(void);

class Maze_Map
{
public:
    struct
    {
        int8_t y = 0, x = 0; // current position
        struct Direction direction;
        int8_t direction_step = 0;
        void step(uint8_t dir)
        {
            y += direction.y * dir;
            x += direction.x * dir;
        }
    }position;

    union
    {
        uint64_t big = 0; 
        uint8_t little[8];
    }position_map, left_map, up_map;

    void fix_maps(void);
    void shift_maps(int8_t dir);
    bool can_go_right(void);
    bool can_go_front(void);
    bool can_go_left(void);
    void change_map(enum Maps map, bool state, struct Direction position);
    bool get_at_position(enum Maps map, struct Direction position);
    Maze_Map(void)
    {
        //fix_maps(); // add this one again when the position works
    }

};

extern Maze_Map maze;
