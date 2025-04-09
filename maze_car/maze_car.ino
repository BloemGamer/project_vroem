//libraries
#include "motor_shield.h"
#include "bluetooth.h"
#include "Servo.h"
#include "led_matrix.h"
// #include "test.h"
#include <NewPing.h>


//constants -> inputs
#define IR_SENSOR_LEFT 28
#define IR_SENSOR_RIGHT 29
#define DISTANCE_SENSOR_LEFT_ECHO A0
#define DISTANCE_SENSOR_RIGHT_ECHO A2
#define DISTANCE_SENSOR_FRONT_ECHO A4

//constants -> outputs
#define DISTANCE_SENSOR_LEFT_TRIG A1
#define DISTANCE_SENSOR_RIGHT_TRIG A3
#define DISTANCE_SENSOR_FRONT_TRIG A5

//tuning
#define MAX_ULTRASONIC_WALL_DISTANCE_FRONT 10
#define MAX_ULTRASONIC_WALL_DISTANCE_SIDES 5
#define INPUT_AMOUTH 5
#define OUTPUT_AMOUTH 3
#define CAR_WIDTH 17
#define PATH_WIDTH 27
#define STANDARD_FORWARD_SPEED 130
#define QUARTER_DELAY 750
#define HALF_DELAY 1500
#define STRAFE_DELAY 50
#define STRAFE_CONSTANT 50

//bluetooth instructions
#define BLUETOOTH_FORWARDS 'f'
#define BLUETOOTH_BACKWARDS 'b'
#define BLUETOOTH_ROTATE_LEFT 'l'
#define BLUETOOTH_ROTATE_RIGHT 'r'
#define BLUETOOTH_STRAFE_RIGHT 'y'
#define BLUETOOTH_STRAFE_LEFT 'x'
#define BLUETOOTH_SPEED_UP 'u'
#define BLUETOOTH_SPEED_DOWN 'd'


#ifdef BLUETOOTH
  Blue_Tooth bluetooth;
  char instruction;
#elif defined TEST_SENSORS

#else

#endif

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
bool turning = false;
unsigned int measured_ultrasonic_distance_left, measured_ultrasonic_distance_right, measured_ultrasonic_distance_front;
unsigned long delay_time = 0;



Motor_Shield motor_shield;
Led_Matrix led_matrix;

void setup(void)
{
  Serial.begin(9600);
  Serial1.begin(9600);
  for(int8_t i = 0; i < INPUT_AMOUTH; i++)
  {
    pinMode(inputs[i], INPUT);
  }
  for(int8_t i = 0; i < OUTPUT_AMOUTH; i++)
  {
    pinMode(outputs[i], OUTPUT);
  }
  // motor_shield.change_motor_direction(GO_FORWARD);
  motor_shield.set_speed(STANDARD_FORWARD_SPEED, STANDARD_FORWARD_SPEED, STANDARD_FORWARD_SPEED, STANDARD_FORWARD_SPEED);
}

void loop(void)
{
#ifdef BLUETOOTH
  if (Serial1.available())
  {
    instruction = bluetooth.bluetooth_read_char();
  }
  // Serial.println(instruction);
  // bluetooth.bluetooth_read_string();
  if(instruction == BLUETOOTH_SPEED_UP)
  {
    motor_shield.set_speed(255, 255, 255, 255);
  }
  if(instruction == BLUETOOTH_SPEED_DOWN)
  {
    motor_shield.set_speed(100, 100, 100, 100);
  }
  if(instruction == BLUETOOTH_FORWARDS)
  {
    motor_shield.change_motor_direction(GO_FORWARD);
  }
  if(instruction == BLUETOOTH_BACKWARDS)
  {
    motor_shield.change_motor_direction(GO_BACK);
  }
  if(instruction == BLUETOOTH_ROTATE_LEFT)
  {
    motor_shield.change_motor_direction(GO_LEFT);
  }
  if(instruction == BLUETOOTH_ROTATE_RIGHT)
  {
    motor_shield.change_motor_direction(GO_RIGHT);
  }
  if(instruction == BLUETOOTH_STRAFE_RIGHT)
  {
    motor_shield.change_motor_direction(FORWARD, BACKWARD, BACKWARD, FORWARD);
  }
  if(instruction == BLUETOOTH_STRAFE_LEFT)
  {
    motor_shield.change_motor_direction(BACKWARD, FORWARD, FORWARD, BACKWARD);
  }
  if(instruction == 's')
  {
    motor_shield.change_motor_direction(STOP);
  }

#elif defined TEST_SENSORS // BLUETOOTH
  motor_shield.change_motor_direction(STOP);
  take_measurements();
  led_matrix.show_sensors();

#else // TEST & BLUETOOTH
  take_measurements();
  if(delay_time < millis()) // if there is enough time between starting the turn and now/if not turning
  {
    if(turning) // reset the speed and direction
    {
      Serial.println(turning);
      motor_shield.change_motor_direction(GO_FORWARD);
      motor_shield.set_speed(STANDARD_FORWARD_SPEED, STANDARD_FORWARD_SPEED, STANDARD_FORWARD_SPEED, STANDARD_FORWARD_SPEED);
      turning = false; // make sure it only resets the speed and direction once
    }
    if(measured_ultrasonic_distance_front < MAX_ULTRASONIC_WALL_DISTANCE_FRONT) // if to close to front wall
    {
      if((measured_ultrasonic_distance_left + measured_ultrasonic_distance_right + CAR_WIDTH) > PATH_WIDTH) // if there is a path right or left
      {
        //there is a free space next to the car
        if(measured_ultrasonic_distance_right > measured_ultrasonic_distance_left)
        {
          //rotate 90 degrees right and continue moving
          motor_shield.change_motor_direction(FORWARD, BACKWARD, FORWARD, BACKWARD);
          delay_time = millis() + QUARTER_DELAY; // start the timer for when to reset and check again
          turning = true;
        }
        else
        {
          //rotate 90 degrees left and continue moving
          motor_shield.change_motor_direction(BACKWARD, FORWARD, BACKWARD, FORWARD);
          delay_time = millis() + QUARTER_DELAY; // start the timer for when to reset and check again
          turning = true;
        }
      }
      else
      {
        //rotate 180 degrees and continue moving
        motor_shield.change_motor_direction(BACKWARD, FORWARD, BACKWARD, FORWARD);
        delay_time = millis() + HALF_DELAY; // start the timer for when to reset and check again
        turning = true;
      }
    }
  
  
    else if(measured_ultrasonic_distance_left < MAX_ULTRASONIC_WALL_DISTANCE_SIDES)
    {
      //strafe right
      //motor_shield.change_speed(-STRAFE_CONSTANT, 0, 0, -STRAFE_CONSTANT); // Hoe TF werkt dit??
      motor_shield.set_speed(STANDARD_FORWARD_SPEED, STANDARD_FORWARD_SPEED - STRAFE_CONSTANT, STANDARD_FORWARD_SPEED - STRAFE_CONSTANT, STANDARD_FORWARD_SPEED);
      delay_time = millis() + STRAFE_DELAY; // start the timer for when to reset and check again
      turning = true;
    }
    else if(measured_ultrasonic_distance_right < MAX_ULTRASONIC_WALL_DISTANCE_SIDES)
    {
      //strafe left
      //motor_shield.change_speed(0, -STRAFE_CONSTANT, -STRAFE_CONSTANT, 0);
      motor_shield.set_speed(STANDARD_FORWARD_SPEED - STRAFE_CONSTANT, STANDARD_FORWARD_SPEED, STANDARD_FORWARD_SPEED, STANDARD_FORWARD_SPEED - STRAFE_CONSTANT);
      delay_time = millis() + STRAFE_DELAY; // start the timer for when to reset and check again
      turning = true;
    }
  }
  else
  {
    // functions only run while turning or strafing
  }
  led_matrix.show_sensors(); 
#endif // NOT BLUETOOTH && NOT TEST
}

void take_measurements(void)
{
  ir_right_trigged = !digitalRead(IR_SENSOR_RIGHT);
  ir_left_trigged = !digitalRead(IR_SENSOR_LEFT);
  measured_ultrasonic_distance_left = sonarLeft.ping_cm();
  measured_ultrasonic_distance_right = sonarRight.ping_cm();
  measured_ultrasonic_distance_front = sonarFront.ping_cm();
#ifdef DEBUG_MODE
  Serial.print("Left: ");
  Serial.println(measured_ultrasonic_distance_left);
  Serial.print("Right: ");
  Serial.println(measured_ultrasonic_distance_right);
  Serial.print("Front: ");
  Serial.println(measured_ultrasonic_distance_front);
  delay(100);
#endif // DEBUG_MODE
}
