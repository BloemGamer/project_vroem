#pragma once
#include <stdint.h>

// #define DEBUG_MODE
// #define BLUETOOTH
// #define TEST_SENSORS
#define SOLVE_MAZE

#define M_LF 0
#define M_RF 1
#define M_LB 2
#define M_RB 3

enum Motor_Directions
{
    BREAK = 0b00,
    FORWARD = 0b01,
    BACKWARD = 0b10
};

#define GO_FORWARD FORWARD, FORWARD, FORWARD, FORWARD
#define GO_BACK BACKWARD, BACKWARD, BACKWARD, BACKWARD
#define TURN_LEFT BACKWARD, FORWARD, BACKWARD, FORWARD
#define TURN_RIGHT FORWARD, BACKWARD, FORWARD, BACKWARD
#define STOP BREAK, BREAK, BREAK, BREAK


class Motor_Shield
{
private:
    uint8_t motor_state = 0;
    void update_motor_directions(); // shifts the motor_state to the motor shield

public:
    void go_old_speed(void);
    void set_speed(uint8_t speed_m1, uint8_t speed_m2, uint8_t speed_m3, uint8_t speed_m4);
    void set_speed(int8_t motor, uint8_t speed_m);
    void change_motor_direction(enum Motor_Directions dir1, enum Motor_Directions dir2, enum Motor_Directions dir3, enum Motor_Directions dir4);

    Motor_Shield(void);
    ~Motor_Shield(void);
};
