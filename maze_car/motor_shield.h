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

#define BREAK 0b00
#define FORWARD 0b01
#define BACKWARD 0b10

#define GO_FORWARD FORWARD, FORWARD, FORWARD, FORWARD
#define GO_BACK BACKWARD, BACKWARD, BACKWARD, BACKWARD
#define TURN_LEFT BACKWARD, FORWARD, BACKWARD, FORWARD
#define TURN_RIGHT FORWARD, BACKWARD, FORWARD, BACKWARD
#define STOP BREAK, BREAK, BREAK, BREAK


class Motor_Shield
{
private:
    uint8_t speed_motors[4];
    volatile uint16_t* motors_speed[4];
    uint8_t motor_state = 0;
    void store_old_motor_state(void);

public:
    uint8_t speed_motors_old[5];
    void go_old_speed(void);
    void set_speed(uint8_t speed_m1, uint8_t speed_m2, uint8_t speed_m3, uint8_t speed_m4);
    void set_speed(uint8_t* speed);
    void set_speed(int8_t motor, uint8_t speed_m);
    void change_speed(int16_t speed_m1, int16_t speed_m2, int16_t speed_m3, int16_t speed_m4); // This one does not work as intended at the moment
    void change_speed(int8_t motor, int16_t speed_m); // This one does not work as intended at the moment
    void update_motor_directions(); // shifts the motor_state to the motor shield

    void change_motor_direction(uint8_t dir1, uint8_t dir2, uint8_t dir3, uint8_t dir4);

    Motor_Shield(void);
    ~Motor_Shield(void);
};
