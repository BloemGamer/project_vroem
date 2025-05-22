#include <stdint.h>

#define right_place_in_map(a) a

#define DOWN 0
#define LEFT 1
#define UP 2
#define RIGHT 3



//Direction dir_arr[4] = {{1, 0}, {0, -1}, {-1, 0}, {0, 1}};

struct Direction
{
    int8_t y;
    int8_t x;

    Direction(int8_t y_val = 1, int8_t x_val = 0) : y(y_val), x(x_val) {}
};

extern Direction dir_arr[4];

void fix_position(void);

struct Maze_Map
{
    struct
    {
        int8_t y = 0, x = 0; // current position
        struct Direction direction;
        int8_t direction_step = 0;
        void step(void)
        {
            y += direction.y;
            x += direction.x;
        }
    }position;

    union
    {
        uint64_t big = 0; 
        uint8_t little[8];
    }position_map, right_map, up_map;

    void fix_maps(void);
    void shift_maps(int8_t dir);
    bool can_go_right(void);
    bool can_go_front(void);
    bool can_go_left(void);

};

extern Maze_Map maze;
