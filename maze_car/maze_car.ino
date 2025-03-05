//libraries
#include "motor_shield.h"
#include "Servo.h"
// #include "test.h"
#include <NewPing.h>
//constants -> inputs
#define IR_SENSOR_LEFT 51
#define IR_SENSOR_RIGHT 52
#define DISTANCE_SENSOR_LEFT_ECHO A0
#define DISTANCE_SENSOR_RIGHT_ECHO A2
#define DISTANCE_SENSOR_FRONT_ECHO A4
//constants -> outputs
#define DISTANCE_SENSOR_LEFT_TRIG A1
#define DISTANCE_SENSOR_RIGHT_TRIG A3
#define DISTANCE_SENSOR_FRONT_TRIG A5
//tuning
#define MAX_ULTRASONIC_WALL_DISTANCE_FRONT 10
#define MAX_ULTRASONIC_WALL_DISTANCE_SIDES 10
#define INPUT_AMOUTH 5
#define OUTPUT_AMOUTH 3
#define CAR_WIDTH 20
#define PATH_WIDTH 30
#define STANDARD_FORWARD_SPEED (uint8_t)255
//sensor array
const int8_t inputs[INPUT_AMOUTH] = {IR_SENSOR_LEFT, IR_SENSOR_RIGHT, DISTANCE_SENSOR_LEFT_ECHO, DISTANCE_SENSOR_RIGHT_ECHO, DISTANCE_SENSOR_FRONT_ECHO};
const int8_t outputs[OUTPUT_AMOUTH] = {DISTANCE_SENSOR_LEFT_TRIG, DISTANCE_SENSOR_RIGHT_TRIG, DISTANCE_SENSOR_FRONT_TRIG};
//library objects
NewPing sonarLeft(DISTANCE_SENSOR_LEFT_TRIG, DISTANCE_SENSOR_LEFT_ECHO, 300);
NewPing sonarRight(DISTANCE_SENSOR_RIGHT_TRIG, DISTANCE_SENSOR_RIGHT_ECHO, 300);
NewPing sonarFront(DISTANCE_SENSOR_FRONT_TRIG, DISTANCE_SENSOR_FRONT_ECHO, 300);
//variables
bool ir_right_trigged = false;
bool ir_left_trigged = false;
long duration1, duration2, duration3;
unsigned int measured_ultrasonic_distance_left, measured_ultrasonic_distance_right, measured_ultrasonic_distance_front;

Motor_Shield motor_shield;

void setup()
{
  Serial.begin(9600);
  for(int8_t i = 0; i < INPUT_AMOUTH; i++)
  {
    pinMode(inputs[i], INPUT);
  }
  for(int8_t i = 0; i < OUTPUT_AMOUTH; i++)
  {
    pinMode(outputs[i], OUTPUT);
  }
  motor_shield.set_speed(STANDARD_FORWARD_SPEED, STANDARD_FORWARD_SPEED, STANDARD_FORWARD_SPEED, STANDARD_FORWARD_SPEED);
}

void loop()
{
  take_measurements();
  motor_shield.update_speed();
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

void take_measurements()
{
  ir_right_trigged = digitalRead(IR_SENSOR_RIGHT);
  ir_left_trigged = digitalRead(IR_SENSOR_LEFT);
  measured_ultrasonic_distance_left = sonarLeft.ping_cm();
  measured_ultrasonic_distance_right = sonarRight.ping_cm();
  measured_ultrasonic_distance_front = sonarFront.ping_cm();
  Serial.print("Left: ");
  Serial.println(measured_ultrasonic_distance_left);
  Serial.print("Right: ");
  Serial.println(measured_ultrasonic_distance_right);
  Serial.print("Front: ");
  Serial.println(measured_ultrasonic_distance_front);
  delay(100);
}