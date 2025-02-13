//libraries
#include "motor_shield.h"
#include "servo.h"

//constants
//sensors
#define IR_SENSOR_LEFT 
#define IR_SENSOR_RIGHT 
#define DISTANCE_SENSOR_LEFT_ECHO
#define DISTANCE_SENSOR_LEFT_TRIG
#define DISTANCE_SENSOR_RIGHT_ECHO
#define DISTANCE_SENSOR_RIGHT_TRIG
#define DISTANCE_SENSOR_FRONT_ECHO
#define DISTANCE_SENSOR_FRONT_TRIG
//tuning
#define MAX_ULTRASONIC_WALL_DISTANCE 
#define MAX_LIGHT_SENSOR_DISTANCE
#define SENSOR_AMOUTH
//sensor array
const int sensors[SENSOR_AMOUTH] = (IR_SENSOR_LEFT, IR_SENSOR_RIGHT, DISTANCE_SENSOR_LEFT_ECHO, DISTANCE_SENSOR_LEFT_TRIG, DISTANCE_SENSOR_RIGHT_ECHO, DISTANCE_SENSOR_RIGHT_TRIG, DISTANCE_SENSOR_FRONT_ECHO, DISTANCE_SENSOR_FRONT_TRIG);

//variables
bool ir_right_trigged = false;
bool ir_left_trigged = false;
long duration1, duration2, duration3;
int8_t measured_ultrasonic_distance_left, measured_ultrasonic_distance_right, measured_ultrasonic_distance_front;

void setup()
{
  for(int8_t i = 0; i < SENSOR_AMOUTH; i++)
  {
    pinMode(sensors[i]);
  }
}

void loop()
{
  take_measurements();
}

take_measurements()
{
  ir_right_trigged = digitalRead(IR_SENSOR_RIGHT);
  ir_left_trigged = digitalRead(IR_SENSOR_LEFT);
}