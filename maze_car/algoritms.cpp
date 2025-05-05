#include <Arduino.h>
#include "algoritms.h"
#include "accelerometer.h"

// This code is writen late at night, it is not recomended to even try to understand what is happening,
// there is a possibility I will try to make this readable, and maybe even documentation, but prob not


#define BLOCK_LENGHT 30

Maze_Map maze;
extern Accelerometer accelerometer;
Direction dir_arr[4] = {{1, 0}, {0, -1}, {-1, 0}, {0, 1}};

void fix_position(void)
{
    if(accelerometer.forward_position > BLOCK_LENGHT)
    {
        accelerometer.forward_position -= BLOCK_LENGHT;
        maze.position.step();
        maze.fix_maps();
    }
}

void Maze_Map::fix_maps(void)
{
    if(position.y > 7) { shift_maps(UP); position.y--; }
    if(position.y < 0) { shift_maps(DOWN); position.y++; }
    if(position.x > 7) { shift_maps(LEFT); position.x--; }
    if(position.x < 0) { shift_maps(RIGHT); position.x++; }

    position_map.little[right_place_in_map(position.y)] |= 1 << position.x;
    right_map.little[right_place_in_map(position.y)] |= 1 << position.x;
    up_map.little[right_place_in_map(position.y)] |= 1 << position.x;
}

void Maze_Map::shift_maps(int8_t dir)
{
    switch (dir)
    {
        case RIGHT:
            position_map.big >>= 1;
            break;
        case LEFT:
            position_map.big <<= 1;
            break;
        case UP:
            position_map.big <<= 8;
            break;
        case DOWN:
            position_map.big >>= 8;
            break;
    }
    return;
}
