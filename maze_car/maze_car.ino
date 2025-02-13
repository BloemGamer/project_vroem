//libraries
#include "motor_shield.h"
#include "servo.h"

//constants -> inputs
#define IR_SENSOR_LEFT 
#define IR_SENSOR_RIGHT 
#define DISTANCE_SENSOR_LEFT_ECHO
#define DISTANCE_SENSOR_RIGHT_ECHO
#define DISTANCE_SENSOR_FRONT_ECHO
//constants -> outputs
#define DISTANCE_SENSOR_LEFT_TRIG
#define DISTANCE_SENSOR_RIGHT_TRIG
#define DISTANCE_SENSOR_FRONT_TRIG
//tuning
#define MAX_ULTRASONIC_WALL_DISTANCE_FRONT
#define MAX_ULTRASONIC_WALL_DISTANCE_SIDES
#define MAX_LIGHT_SENSOR_DISTANCE
#define INPUT_AMOUTH
#define OUTPUT_AMOUTH
#define CAR_WIDTH
#define PATH_WIDTH
//sensor array
const int8_t inputs[INPUT_AMOUTH] = (IR_SENSOR_LEFT, IR_SENSOR_RIGHT, DISTANCE_SENSOR_LEFT_ECHO, DISTANCE_SENSOR_RIGHT_ECHO, DISTANCE_SENSOR_FRONT_ECHO);
const int8_t outputs[OUTPUT_AMOUTH] = (DISTANCE_SENSOR_LEFT_TRIG, DISTANCE_SENSOR_RIGHT_TRIG, DISTANCE_SENSOR_FRONT_TRIG);

//variables
bool ir_right_trigged = false;
bool ir_left_trigged = false;
long duration1, duration2, duration3;
int8_t measured_ultrasonic_distance_left, measured_ultrasonic_distance_right, measured_ultrasonic_distance_front;

void setup()
{
  for(int8_t i = 0; i < INPUT_AMOUTH; i++)
  {
    pinMode(inputs[i], INPUT);
  }
  for(int8_t i = 0; i < OUTPUT_AMOUTH; i++)
  {
    pinMode(outputs[i], INPUT);
  }
}

void loop()
{
  take_measurements();
  if (measured_ultrasonic_distance_front < MAX_ULTRASONIC_WALL_DISTANCE_FRONT)
  {
    //stop moving
    if((measured_ultrasonic_distance_left + measured_ultrasonic_distance_right + CAR_WIDTH) > PATH_WIDTH)
    {
      //there is a free space next to the car
      if(measured_ultrasonic_distance_right > measured_ultrasonic_distance_left)
      {
        //rotate 90 degrees right and continue moving
      }
      else
      {
        //rotate 90 degrees left and continue moving
      }
    }
    else
    {
      //rotate 180 degrees and continue moving
    }
  }
  if (measured_ultrasonic_distance_left < MAX_ULTRASONIC_WALL_DISTANCE_SIDES)
  {
    //strafe right 
  }
  if (measured_ultrasonic_distance_right < MAX_ULTRASONIC_WALL_DISTANCE_SIDES)
  {
    //strafe left
  }
}

take_measurements()
{
  ir_right_trigged = digitalRead(IR_SENSOR_RIGHT);
  ir_left_trigged = digitalRead(IR_SENSOR_LEFT);
  measured_ultrasonic_distance_left = readDistance(DISTANCE_SENSOR_LEFT_TRIG, DISTANCE_SENSOR_LEFT_ECHO);
  measured_ultrasonic_distance_right = readDistance(DISTANCE_SENSOR_RIGHT_TRIG, DISTANCE_SENSOR_RIGHT_ECHO);
  measured_ultrasonic_distance_front = readDistance(DISTANCE_SENSOR_FRONT_TRIG, DISTANCE_SENSOR_FRONT_ECHO);
}

int8_t readDistance(int8_t triggerPin, int8_t echoPin)
{
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2;
  return distance;
}