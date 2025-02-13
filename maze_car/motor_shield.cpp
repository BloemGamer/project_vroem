#include "Arduino.h"
#include "motor_shield.h"

void Motor_Shield::set_speed(uint8_t speed_lf, uint8_t speed_rf, uint8_t speed_lb, uint8_t speed_rb)
{
  speed_motors[M_LF] = speed_lf;
  speed_motors[M_RF] = speed_rf;
  speed_motors[M_LB] = speed_lb;
  speed_motors[M_RB] = speed_rb;
}

void Motor_Shield::set_speed(uint8_t motor, uint8_t speed_m)
{
  speed_motors[motor] = speed_m;
}

void Motor_Shield::update_speed()
{

}





Motor_shield::Motor_Shield()
{
// zet de pinnen op input/output van wat ze moeten
  pinMode(PIN_M1, OUTPUT);
  pinMode(PIN_M2, OUTPUT);
  pinMode(PIN_M3, OUTPUT);
  pinMode(PIN_M4, OUTPUT);

}

Motor_Shield::~Motor_Shield()
{

}