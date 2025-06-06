#include <Arduino.h>
#include "algoritms.h"
#include "accelerometer.h"

// This code is writen late at night, it is not recomended to even try to understand what is happening,
// there is a possibility I will try to make this readable, and maybe even documentation, but prob not


#define BLOCK_LENGHT 30

extern bool ir_right_trigged;
extern bool ir_left_trigged;
extern Maze_Map maze;
Direction dir_arr[4] = {{1, 0}, {0, -1}, {-1, 0}, {0, 1}};
extern Accelerometer accelerometer;
extern unsigned int measured_ultrasonic_distance_left, measured_ultrasonic_distance_right, measured_ultrasonic_distance_front;

void fix_position(void)
{
    Serial.print(accelerometer.get_forwards_movement());
    if(accelerometer.get_forwards_movement() > BLOCK_LENGHT)
    {
        accelerometer.forward_position -= BLOCK_LENGHT;
        maze.position.step(1);
        maze.fix_maps();
    }
    if(accelerometer.get_forwards_movement() < BLOCK_LENGHT)
    {
        accelerometer.forward_position += BLOCK_LENGHT;
        maze.position.step(-1);
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

bool Maze_Map::can_go_right(void)
{
    return 
    (
        (measured_ultrasonic_distance_right > BLOCK_LENGHT) && 
        (!(position_map.little[right_place_in_map(position.y - position.direction.x)] & 1 << (position.x - position.direction.y))) && // prob not to safe, but checks if we've been at the place on the right
        (!ir_right_trigged)
    );
}

bool Maze_Map::can_go_front(void)
{
    return 
    (
        (measured_ultrasonic_distance_front > BLOCK_LENGHT) && 
        (!(position_map.little[right_place_in_map(position.y + position.direction.y)] & 1 << (position.x + position.direction.x))) // prob not to safe, but checks if we've been at the place on the front
    );
}

bool Maze_Map::can_go_left(void)
{
    return 
    (
        (measured_ultrasonic_distance_left > BLOCK_LENGHT) && 
        (!(position_map.little[right_place_in_map(position.y + position.direction.x)] & 1 << (position.x + position.direction.y))) && // prob not to safe, but checks if we've been at the place on the left
        (!ir_left_trigged)
    );
}
