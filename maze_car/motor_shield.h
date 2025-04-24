#pragma once
#include <stdint.h>

// #define DEBUG_MODE
// #define BLUETOOTH
// #define TEST_SENSORS


#define LF1 1
#define LF2 4
#define RF1 2
#define RF2 3
#define LB1 7
#define LB2 5
#define RB1 0
#define RB2 6

#define M_LF 0
#define M_RF 1
#define M_LB 2
#define M_RB 3

#define MOTORLATCH 12
#define CLOCK_PIN 4
#define ENABLE_PIN 7
#define DATA_PIN 8

#define BREAK 0b00
#define FORWARD 0b01
#define BACKWARD 0b10

#define LF_PIN 11
#define RF_PIN 3
#define LB_PIN 5
#define RB_PIN 6

#define GO_FORWARD FORWARD, FORWARD, FORWARD, FORWARD
#define GO_BACK BACKWARD, BACKWARD, BACKWARD, BACKWARD
#define TURN_LEFT FORWARD, BACKWARD, FORWARD, BACKWARD
#define TURN_RIGHT BACKWARD, FORWARD, BACKWARD, FORWARD
#define STOP BREAK, BREAK, BREAK, BREAK


#define SPEED_RF OCR1A
#define SPEED_LF OCR3C
#define SPEED_LB OCR4A
#define SPEED_RB OCR3A



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
