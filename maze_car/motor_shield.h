#pragma once
#include <stdint.h>


#define LF1 2
#define LF2 3
#define RF1 1
#define RF2 4
#define LB1 0
#define LB2 6
#define RB1 5
#define RB2 7

#define M_LF 0
#define M_RF 1
#define M_LB 2
#define M_RB 3

#define CLOCK_PIN 4



class Motor_Shield
{
private:
  uint8_t speed_motors[4];
  uint8_t motor_state = 0;

public:
  void set_speed(uint8_t speed_m1, uint8_t speed_m2, uint8_t speed_m3, uint8_t speed_m4);
  void set_speed(uint8_t motor, uint8_t speed_m);
  void change_speed(uint8_t speed_m1, uint8_t speed_m2, uint8_t speed_m3, uint8_t speed_m4);
  void change_speed(uint8_t motor, uint8_t speed_m);
  void update_speed();
  void update_single_speed();
  uint8_t motor_state_(uint8_t motor, uint8_t direction);

  Motor_Shield();
  ~Motor_Shield();
};