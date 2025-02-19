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

void Motor_Shield::change_speed(uint8_t speed_lf, uint8_t speed_rf, uint8_t speed_lb, uint8_t speed_rb)
{
  speed_motors[M_LF] += speed_lf;
  speed_motors[M_RF] += speed_rf;
  speed_motors[M_LB] += speed_lb;
  speed_motors[M_RB] += speed_rb;
}

void Motor_Shield::change_speed(uint8_t motor, uint8_t speed_m)
{
  speed_motors[motor] += speed_m;
}

void Motor_Shield::update_speed()
{
  pinMode(7, OUTPUT);
  for(uint8_t m = 0; m < 4; m++)
  {
    motor_state_(m, 1);
    pinMode(CLOCK_PIN, OUTPUT);
    pinMode(12, OUTPUT);
    pinMode(CLOCK_PIN, OUTPUT);
    pinMode(8, OUTPUT);
    
    
    digitalWrite(7, LOW);
    digitalWrite(12, LOW);
    shiftOut(8, CLOCK_PIN, MSBFIRST, motor_state);
    digitalWrite(12, HIGH);
    // digitalWrite(7, HIGH);
  }
}

void Motor_Shield::update_single_speed()
{
  
}

void Motor_Shield::motor_state_(uint8_t motor, uint8_t direction)
{
  motor_state = 0;
  delay(250);
  switch(motor)
  {
    case M_LF:
      motor_state &= ~(1 << LF1) & ~(1 << LF2);
      motor_state |= 1 << LF1;
      motor_state &= ~(1 << LF2);
      return;
    case M_RF:
      motor_state &= ~(1 << RF1) & ~(1 << RF2);
      motor_state |= 1 << RF1;
      motor_state &= ~(1 << RF2);
      return;
    case M_LB:
      motor_state &= ~(1 << LB1) & ~(1 << LB2);
      motor_state |= 1 << LB1;
      motor_state &= ~(1 << LB2);
      return;
    case M_RB:
      motor_state &= ~(1 << RB1) & ~(1 << RB2);
      motor_state |= 1 << RB1;
      motor_state &= ~(1 << RB2);
      return;
  }
}




Motor_Shield::Motor_Shield()
{
// zet de pinnen op input/output van wat ze moeten
  pinMode(LF1, OUTPUT);
  pinMode(LF2, OUTPUT);
  pinMode(RF1, OUTPUT);
  pinMode(RF2, OUTPUT);
  pinMode(LB1, OUTPUT);
  pinMode(LB2, OUTPUT);
  pinMode(RB1, OUTPUT);
  pinMode(RB2, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);

}

Motor_Shield::~Motor_Shield()
{
  
}

