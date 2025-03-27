#include "Arduino.h"
#include <stdint.h>
#include <string.h>
#include "motor_shield.h"

// bitwise black magic
// every motor has a forwards and backwards bit in the motor_state uint8_t, and this function fixes that
// only one of those two bits is on, and the right one, and is break, both are off.
#define change_motor_dir(a, b, dir) \
switch(dir) \
{ \
  case(FORWARD): \
    motor_state |= 1 << (a); \
    motor_state &= ~(1 << (b)); break; \
  case(BACKWARD): \
    motor_state &= ~(1 << (a)); \
    motor_state |= 1 << (b); break; \
  case(BREAK): \
    motor_state &= ~(1 << (a)); \
    motor_state &= ~(1 << (b)); break; \
} 

// Set the PWM pin of the motors on the right value
void Motor_Shield::set_speed(uint8_t speed_lf, uint8_t speed_rf, uint8_t speed_lb, uint8_t speed_rb)
{
#ifdef ARDUINO
  analogWrite(LF_PIN, speed_lf);
  speed_motors[M_LF] = speed_lf;
  analogWrite(RF_PIN, speed_rf);
  speed_motors[M_RF] = speed_rf;
  analogWrite(LB_PIN, speed_lb);
  speed_motors[M_LB] = speed_lb;
  analogWrite(RB_PIN, speed_rb);
  speed_motors[M_RB] = speed_rb;
#endif // ARDUINO
}

void Motor_Shield::set_speed(uint8_t* speed)
{
#ifdef ARDUINO
  analogWrite(LF_PIN, speed[M_LF]);
  speed_motors[M_LF] = speed[M_LF]);
  analogWrite(RF_PIN, speed[M_RF]);
  speed_motors[M_RF] = speed[M_RF]);
  analogWrite(LB_PIN, speed[M_LB]);
  speed_motors[M_LB] = speed[M_LB]);
  analogWrite(RB_PIN, speed[M_RB]);
  speed_motors[M_RB] = speed[M_RB]);
#endif // ARDUINO
}

void Motor_Shield::set_speed(int8_t motor, uint8_t speed_m)
{
#ifdef ARDUINO
  switch(motor)
  {
    case(M_LF):
      analogWrite(LF_PIN, speed_m);
      speed_motors[M_LF] = speed_m; break;
    case(M_RF):
      analogWrite(RF_PIN, speed_m);
      speed_motors[M_RF] = speed_m; break;
    case(M_LB):
      analogWrite(LB_PIN, speed_m);
      speed_motors[M_LB] = speed_m; break;
    case(M_RB):
      analogWrite(RB_PIN, speed_m);
      speed_motors[M_RB] = speed_m; break;
 }
#endif // ARDUINO 
}

// changing the speed
const uint8_t* Motor_Shield::change_speed(int8_t speed_lf, int8_t speed_rf, int8_t speed_lb, int8_t speed_rb)
{
#ifdef ARDUINO
  speed_motors[M_LF] += min(255, speed_lf);
  analogWrite(LF_PIN, speed_motors[M_LF]);
  speed_motors[M_RF] += min(255, speed_rf);
  analogWrite(RF_PIN, speed_motors[M_RF]);
  speed_motors[M_LB] += min(255, speed_lb);
  analogWrite(LB_PIN, speed_motors[M_LB]);
  speed_motors[M_RB] += min(255, speed_rb);
  analogWrite(RB_PIN, speed_motors[M_RB]);
#endif // ARDUINO
  memcpy((void*)speed_motors, (void*)speed_motors_old, 4 * sizeof(uint8_t));
  return speed_motors_old;
}

void Motor_Shield::change_speed(int8_t motor, int8_t speed_m)
{
// #ifdef ARDUINO
  switch(motor)
  {
    case(M_LF):
      speed_motors[M_LF] += min(255, speed_m);
      analogWrite(LF_PIN, speed_motors[M_LF]);
      break;
    case(M_RF):
      speed_motors[M_RF] += min(255, speed_m);
      analogWrite(RF_PIN, speed_motors[M_RF]);
      break;
    case(M_LB):
      speed_motors[M_LB] += min(255, speed_m);
      analogWrite(LB_PIN, speed_motors[M_LB]);
      break;
    case(M_RB):
      speed_motors[M_RB] += min(255, speed_m);
      analogWrite(RB_PIN, speed_motors[M_RB]);
      break;
 }
//#endif
}

void Motor_Shield::update_speed()
{
  digitalWrite(MOTORLATCH, LOW);
  digitalWrite(DATA_PIN, LOW);
  for(uint8_t i = 0; i < 8; i++)
  {
    digitalWrite(CLOCK_PIN, LOW);
    digitalWrite(DATA_PIN, (motor_state & (1 << (7 - i)) ? HIGH : LOW));
    digitalWrite(CLOCK_PIN, HIGH);
  }
  digitalWrite(MOTORLATCH, HIGH);
}

// setting the pins on output and setting the speed on full
Motor_Shield::Motor_Shield(void)
{
// set pins on right mode
  pinMode(MOTORLATCH, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);

  pinMode(11, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);

  // digitalWrite(LF_PIN, HIGH);
  // digitalWrite(RF_PIN, HIGH);
  // digitalWrite(LB_PIN, HIGH);
  // digitalWrite(RB_PIN, HIGH);

  analogWrite(LF_PIN, 255);
  analogWrite(RF_PIN, 255);
  analogWrite(LB_PIN, 255);
  analogWrite(RB_PIN, 255);

  digitalWrite(ENABLE_PIN, LOW);
  
#ifdef ARDUINO // this one is just so my PC doesn't give a lot of errors
  // setting speed on full
  // OCR1A = 255;
  // OCR3C = 255;
  // OCR4A = 255;
  // OCR3A = 255;

  // speed_motors[0] = &OCR1A;
  // speed_motors[1] = &OCR3C;
  // speed_motors[2] = &OCR4A;
  // speed_motors[3] = &OCR3A;
#endif // Arduino
}

// changing the direction of the motor
void Motor_Shield::change_motor_direction(uint8_t dir1, uint8_t dir2, uint8_t dir3, uint8_t dir4)
{
  change_motor_dir(LF1, LF2, dir1);
  change_motor_dir(RF1, RF2, dir2);
  change_motor_dir(LB1, LB2, dir3);
  change_motor_dir(RB1, RB2, dir4);
  update_speed();
}

// destructer of the class, doesn't do anything at the moment
Motor_Shield::~Motor_Shield()
{
  
}

