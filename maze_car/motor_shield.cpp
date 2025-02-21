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
  // pinMode(7, OUTPUT);
  uint8_t motors_d[] = {0x04,0x10,0x01,0x40 };
  uint8_t motors_r[] = {0x08,0x02,0x40,0x80 };
// 0 == RF
// 1 == LF
// 2 == RB
// 3 == LB
  for(uint8_t m = 0; m < 4; m++)
  {
    // Only the first one does something, and it resets when another is written to
    // motor_state = motor_state_(m, 1);
   
    // pinMode(3, OUTPUT);
    // digitalWrite(7, LOW);
    // digitalWrite(12, LOW);
    // shiftOut(8, CLOCK_PIN, MSBFIRST, 255);
    shift_out(motors_r[m]);
    // shiftOut(8, CLOCK_PIN, MSBFIRST, MotorsR[m]);
    // digitalWrite(12, HIGH, motors_d[m]);
    // digitalWrite(7, HIGH);
    delay(250);
  }
  // digitalWrite(ENABLE_PIN, LOW);
}

void Motor_Shield::update_single_speed()
{
  
}

// uint8_t Motor_Shield::motor_state_(uint8_t motor, uint8_t direction)
// {
//   uint8_t state = 0;
//   delay(250);
//   switch(motor)
//   {
//     case M_LF:
//       state &= ~(1 << LF1) & ~(1 << LF2);
//       state |= 1 << LF1;
//       state &= ~(1 << LF2);
//       return state;
//     case M_RF:
//       state &= ~(1 << RF1) & ~(1 << RF2);
//       state |= 1 << RF1;
//       state &= ~(1 << RF2);
//       return state;
//     case M_LB:
//       state &= ~(1 << LB1) & ~(1 << LB2);
//       state |= 1 << LB1;
//       state &= ~(1 << LB2);
//       return state;
//     case M_RB:
//       state &= ~(1 << RB1) & ~(1 << RB2);
//       state |= 1 << RB1;
//       state &= ~(1 << RB2);
//       return state;
//   }
// }

void Motor_Shield::shift_out(uint8_t data)
{
  digitalWrite(MOTORLATCH, LOW);
  digitalWrite(DATA_PIN, LOW);
  for(uint8_t i = 0; i < 8; i++)
  {
    digitalWrite(CLOCK_PIN, LOW);
    // if(data & (1 << (7 - i)))
    //   digitalWrite(DATA_PIN, HIGH);
    // else
    //   digitalWrite(DATA_PIN, LOW);
    digitalWrite(DATA_PIN, (data & (1 << (7 - i)) ? HIGH : LOW));
    digitalWrite(CLOCK_PIN, HIGH);
  }
  digitalWrite(MOTORLATCH, HIGH);
}



Motor_Shield::Motor_Shield()
{
// zet de pinnen op input/output van wat ze moeten
  pinMode(MOTORLATCH, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);


#ifdef ARDUINO
  OCR1A = 255;
  OCR3C = 255;
  OCR4A = 255;
  OCR3A = 255;
#endif
  
}

Motor_Shield::~Motor_Shield()
{
  
}

