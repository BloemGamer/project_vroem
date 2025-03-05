#pragma once
#include <stdint.h>

#define DEBUG_MODE 1

#define RF1 2
#define RF2 3
#define LF1 1
#define LF2 4
#define RB1 0
#define RB2 6
#define LB1 7
#define LB2 5

#define M_LF 0
#define M_RF 1
#define M_LB 2
#define M_RB 3

#define MOTORLATCH 12
#define CLOCK_PIN 4
#define ENABLE_PIN 7
#define DATA_PIN 8

#define FORWARD 0
#define BACKWARD 1
#define BREAK 2



class Motor_Shield
{
private:
  volatile uint16_t *speed_motors[4];
  uint8_t motor_state = 0;

public:
  void set_speed(uint8_t speed_m1, uint8_t speed_m2, uint8_t speed_m3, uint8_t speed_m4);
  void set_speed(uint8_t motor, uint8_t speed_m);
  void change_speed(int8_t speed_m1, int8_t speed_m2, int8_t speed_m3, int8_t speed_m4);
  void change_speed(uint8_t motor, uint8_t speed_m);
  void update_speed(void);
  void shift_out(uint8_t data);
  uint8_t motor_state_(uint8_t motor, uint8_t direction);

  void change_motor_direction(uint8_t dir1, uint8_t dir2, uint8_t dir3, uint8_t dir4);

  Motor_Shield(void);
  ~Motor_Shield(void);
};