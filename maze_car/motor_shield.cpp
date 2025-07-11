#include "Arduino.h"
#include <stdint.h>
#include <string.h>
#include "motor_shield.h"

#define LF1 1
#define LF2 4
#define RF1 2
#define RF2 3
#define LB1 7
#define LB2 5
#define RB1 0
#define RB2 6

#define MOTOR_LATCH 12
#define CLOCK_PIN 4
#define ENABLE_PIN 7
#define DATA_PIN 8

#define LF_PIN 11
#define RF_PIN 3
#define LB_PIN 5
#define RB_PIN 6

void Motor_Shield::set_speed(uint8_t speed_lf, uint8_t speed_rf, uint8_t speed_lb, uint8_t speed_rb)// changing the speed by changing the PWM speed of the pins
{
    analogWrite(LF_PIN, speed_lf);
    analogWrite(RF_PIN, speed_rf);
    analogWrite(LB_PIN, speed_lb);
    analogWrite(RB_PIN, speed_rb);
}

void Motor_Shield::set_speed(int8_t motor, uint8_t speed_m)
{
    switch(motor)
    {
        case(M_LF):
            analogWrite(LF_PIN, speed_m); break;
        case(M_RF):
            analogWrite(RF_PIN, speed_m); break;
        case(M_LB):
            analogWrite(LB_PIN, speed_m); break;
        case(M_RB):
            analogWrite(RB_PIN, speed_m); break;
    }
}

void Motor_Shield::update_motor_directions() // shifts the motor_state to the motor shield
{
    digitalWrite(MOTOR_LATCH, LOW);
    digitalWrite(DATA_PIN, LOW);
    for(uint8_t i = 0; i < 8; i++)
    {
        digitalWrite(CLOCK_PIN, LOW);
        digitalWrite(DATA_PIN, (motor_state & (1 << (7 - i)) ? HIGH : LOW));
        digitalWrite(CLOCK_PIN, HIGH);
    }
    digitalWrite(MOTOR_LATCH, HIGH);
}

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

// changing the direction of the motor
void Motor_Shield::change_motor_direction(enum Motor_Directions dir1, enum Motor_Directions dir2, enum Motor_Directions dir3, enum Motor_Directions dir4)
{
    change_motor_dir(LF1, LF2, dir1);
    change_motor_dir(RF1, RF2, dir2);
    change_motor_dir(LB1, LB2, dir3);
    change_motor_dir(RB1, RB2, dir4);
    update_motor_directions();
}

#undef change_motor_dir

// setting the pins on output and setting the speed on full
Motor_Shield::Motor_Shield(void)
{
    // set pins on right mode
    pinMode(MOTOR_LATCH, OUTPUT);
    pinMode(ENABLE_PIN, OUTPUT);
    pinMode(DATA_PIN, OUTPUT);
    pinMode(CLOCK_PIN, OUTPUT);

    pinMode(11, OUTPUT);
    pinMode(3, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);

    analogWrite(LF_PIN, 130);
    analogWrite(RF_PIN, 130);
    analogWrite(LB_PIN, 130);
    analogWrite(RB_PIN, 130);

    digitalWrite(ENABLE_PIN, LOW);
}

// destructer of the class, doesn't do anything at the moment
Motor_Shield::~Motor_Shield()
{

}

