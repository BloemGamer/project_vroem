#include "Arduino.h"
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
  SPEED_LF = speed_lf;
  SPEED_RF = speed_rf;
  SPEED_LB = speed_lb;
  SPEED_RB = speed_rb;
#endif // ARDUINO
}

void Motor_Shield::set_speed(int8_t motor, uint8_t speed_m)
{
#ifdef ARDUINO
  switch(motor)
  {
    case(M_LF):
      SPEED_LF = speed_m; break;
    case(M_RF):
      SPEED_RF = speed_m; break;
    case(M_LB):
      SPEED_LB = speed_m; break;
    case(M_RB):
      SPEED_RB = speed_m; break;
 }
#endif // ARDUINO 
}

// changing the speed
void Motor_Shield::change_speed(int8_t speed_lf, int8_t speed_rf, int8_t speed_lb, int8_t speed_rb)
{
#ifdef ARDUINO
  SPEED_LF = min(255, SPEED_LF + speed_lf);
  SPEED_RF = min(255, SPEED_RF + speed_rf);
  SPEED_LB = min(255, SPEED_LB + speed_lb);
  SPEED_RB = min(255, SPEED_RB + speed_rb);
#endif // ARDUINO
}

void Motor_Shield::change_speed(int8_t motor, int8_t speed_m)
{
#ifdef ARDUINO
  switch(motor)
  {
    case(M_LF):
      SPEED_LF = (min(255, SPEED_LF + speed_m)); break;
    case(M_RF):
      SPEED_RF = (min(255, SPEED_RF + speed_m)); break;
    case(M_LB):
      SPEED_LB = (min(255, SPEED_LB + speed_m)); break;
    case(M_RB):
      SPEED_RB = (min(255, SPEED_RB + speed_m)); break;
 }
#endif
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

  digitalWrite(ENABLE_PIN, LOW);
#ifdef ARDUINO // this one is just so my PC doesn't give a lot of errors
  // setting speed on full
  OCR1A = 255;
  OCR3C = 255;
  OCR4A = 255;
  OCR3A = 255;

  speed_motors[0] = &OCR1A;
  speed_motors[1] = &OCR3C;
  speed_motors[2] = &OCR4A;
  speed_motors[3] = &OCR3A;
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

