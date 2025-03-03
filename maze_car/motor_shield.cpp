#include "Arduino.h"
#include "motor_shield.h"

// these ones aren't correct and will NOT be used in the final version of the library
uint8_t md[] = {0x04, 0x02, 0x01, 0x20};
uint8_t motors_d[] = {0x04,0x10,0x01,0x40 };
uint8_t motors_r[] = {0x08,0x02,0x40,0x80 };

void Motor_Shield::set_speed(uint8_t speed_lf, uint8_t speed_rf, uint8_t speed_lb, uint8_t speed_rb)
{
  *speed_motors[M_LF] = speed_lf;
  *speed_motors[M_RF] = speed_rf;
  *speed_motors[M_LB] = speed_lb;
  *speed_motors[M_RB] = speed_rb;
}

void Motor_Shield::set_speed(uint8_t motor, uint8_t speed_m)
{
  *speed_motors[motor] = speed_m;
}

void Motor_Shield::change_speed(uint8_t speed_lf, uint8_t speed_rf, uint8_t speed_lb, uint8_t speed_rb)
{
  *speed_motors[M_LF] = max(255, *speed_motors[M_LF] + speed_lf);
  *speed_motors[M_RF] = max(255, *speed_motors[M_RF] + speed_rf);
  *speed_motors[M_LB] = max(255, *speed_motors[M_LB] + speed_lb);
  *speed_motors[M_RB] = max(255, *speed_motors[M_RB] + speed_rb);
}

void Motor_Shield::change_speed(uint8_t motor, uint8_t speed_m)
{
  *speed_motors[motor] += speed_m;
}

void Motor_Shield::update_speed()
{
  uint8_t m_data = 0;
  for(uint8_t m = 0; m < 4; m++)
  {
   m_data |= md[m];
   shift_out(m_data);
   }
}

inline void Motor_Shield::shift_out(uint8_t data)
{
  digitalWrite(MOTORLATCH, LOW);
  digitalWrite(DATA_PIN, LOW);
  for(uint8_t i = 0; i < 8; i++)
  {
    digitalWrite(CLOCK_PIN, LOW);
    digitalWrite(DATA_PIN, (data & (1 << (7 - i)) ? HIGH : LOW));
    digitalWrite(CLOCK_PIN, HIGH);
  }
  digitalWrite(MOTORLATCH, HIGH);
}



Motor_Shield::Motor_Shield(void)
{
// zet de pinnen op input/output van wat ze moeten
  pinMode(MOTORLATCH, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);

  digitalWrite(ENABLE_PIN, LOW);
#ifdef ARDUINO
  OCR1A = 255;
  OCR3C = 255;
  OCR4A = 255;
  OCR3A = 255;
  speed_motors[0] = (uint16_t*)&OCR1A;
  speed_motors[1] = (uint16_t*)&OCR3C;
  speed_motors[2] = (uint16_t*)&OCR4A;
  speed_motors[3] = (uint16_t*)&OCR3A;
#endif
}

Motor_Shield::~Motor_Shield()
{
  
}

