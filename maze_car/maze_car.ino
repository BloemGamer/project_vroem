//libraries
#include <Servo.h>
#include <NewPing.h>

#include "motor_shield.h"
#include "bluetooth.h"
#include "led_matrix.h"
#include "accelerometer.h"
#include "algoritms.h"


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
#define MAX_ULTRASONIC_WALL_DISTANCE_FRONT 7
#define MAX_ULTRASONIC_WALL_DISTANCE_SIDES 5
#define INPUT_AMOUTH 5
#define OUTPUT_AMOUTH 3
#define CAR_WIDTH 17
#define PATH_WIDTH 27
#define STANDARD_FORWARD_SPEED 130
#define TURNING_SPEED 130
#define QUARTER_DELAY 750
#define HALF_DELAY 1500
#define STRAFE_DELAY 75
#define STRAFE_CONSTANT 75



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
int16_t rotation = 0;


Motor_Shield motor_shield;
Led_Matrix led_matrix;
Blue_Tooth bluetooth;
Accelerometer accelerometer;
Maze_Map maze;

void setup(void)
{
    Serial.begin(9600);
    Serial1.begin(9600);
    accelerometer.initializeSerial2(9600);
    for(int8_t i = 0; i < INPUT_AMOUTH; i++)
    {
        pinMode(inputs[i], INPUT);
    }
    for(int8_t i = 0; i < OUTPUT_AMOUTH; i++)
    {
        pinMode(outputs[i], OUTPUT);
    }
    motor_shield.change_motor_direction(GO_FORWARD);
    motor_shield.set_speed(STANDARD_FORWARD_SPEED, STANDARD_FORWARD_SPEED, STANDARD_FORWARD_SPEED, STANDARD_FORWARD_SPEED);
}

void loop(void)
{
#if defined TEST_SENSORS
    motor_shield.change_motor_direction(STOP);
    take_measurements();
    led_matrix.show_sensors();

#elif defined SOLVE_MAZE
    led_matrix.draw(maze.position_map.little);
    //Serial.println(accelerometer.get_forwards_movement());
    bluetooth.bluetooth_send_int64(maze.position_map.big); 
    take_measurements();
    rotation = accelerometer.get_yaw();
    if(rotation < 10 && rotation > -10 && turning) // if done turning
    {
        reset_speed();
    }
    if(!turning) // when not turning
    {
        fix_position();

        if(measured_ultrasonic_distance_left < MAX_ULTRASONIC_WALL_DISTANCE_SIDES)
        {
            strafe_right();
        }
        else if(measured_ultrasonic_distance_right < MAX_ULTRASONIC_WALL_DISTANCE_SIDES)
        {
            strafe_left();
        }

        // if there is a place right, dan go right, else go forward, else go to the left
        if(maze.can_go_right())
        {
            delay(50);
            right_90());
            return;
        }
        else if(maze.can_go_front())
        {
            // just go forward
            return;
        }
        else if(maze.can_go_left())
        {
            delay(50);
            left_90());
            return;
        }
        else // if all things have been tried, just go yolo, I will fix this later, but first the mapping system should work reliable
        // there will be a better new algoritm in the future, I just don't want to write that at the moment, it's late and I'm tired
        {
            if(measured_ultrasonic_distance_front < MAX_ULTRASONIC_WALL_DISTANCE_FRONT) // if too close to front wall
            {
                if((measured_ultrasonic_distance_left + measured_ultrasonic_distance_right + CAR_WIDTH) > PATH_WIDTH) // if there is a path right or left
                {
                    if(measured_ultrasonic_distance_right > measured_ultrasonic_distance_left) //there is a free space next to the car
                    {
                        right_90());
                    }
                    else
                    {
                        left_90());
                    }
                }
                else
                {
                    right_180());
                }
            }

        }
    }


#else // TEST_SENSORS
    // the old function, will prob NOT work anymore, because the inner function will be/are rewritten
    take_measurements();
    // Serial.println(instruction);
    // bluetooth.bluetooth_read_string();
    if(delay_time < millis()) // if there is enough time between starting the turn and now/if not turning
    {
        if(turning) // reset the speed and direction
        {
            stop();
            reset_speed();
            delay(100);
        }
        if(measured_ultrasonic_distance_front < MAX_ULTRASONIC_WALL_DISTANCE_FRONT) // if too close to front wall
        {
            stop();
            if((measured_ultrasonic_distance_left + measured_ultrasonic_distance_right + CAR_WIDTH) > PATH_WIDTH) // if there is a path right or left
            {
                if(measured_ultrasonic_distance_right > measured_ultrasonic_distance_left) //there is a free space next to the car
                {
                    right_90());
                }
                else
                {
                    left_90());
                }
            }
            else
            {
                right_180());
            }
        }


        else if(measured_ultrasonic_distance_left < MAX_ULTRASONIC_WALL_DISTANCE_SIDES)
        {
            strafe_right();
        }
        else if(measured_ultrasonic_distance_right < MAX_ULTRASONIC_WALL_DISTANCE_SIDES)
        {
            strafe_left();
        }
    }
    else
    {
        // functions only run while turning or strafing
    }
    led_matrix.show_sensors(); 
#endif // NOT TEST
}

void take_measurements(void)
{
    ir_right_trigged = !digitalRead(IR_SENSOR_RIGHT);
    ir_left_trigged = !digitalRead(IR_SENSOR_LEFT);
    measured_ultrasonic_distance_left = sonarLeft.ping_cm();
    measured_ultrasonic_distance_right = sonarRight.ping_cm();
    measured_ultrasonic_distance_front = sonarFront.ping_cm();
}

inline void left_90()
{
    accelerometer.yaw_ = -80;
    maze.position.direction_step = (maze.position.direction_step + 3) % 4;
    maze.position.direction = dir_arr[maze.position.direction_step];
    motor_shield.set_speed(TURNING_SPEED, TURNING_SPEED, TURNING_SPEED, TURNING_SPEED);
    motor_shield.change_motor_direction(TURN_LEFT);
    turning = true;
    
}

inline void right_90()
{
    accelerometer.yaw_ = 80;
    rotation = accelerometer.get_yaw();
    maze.position.direction_step = (maze.position.direction_step + 5) % 4;
    maze.position.direction = dir_arr[maze.position.direction_step];
    motor_shield.set_speed(TURNING_SPEED, TURNING_SPEED, TURNING_SPEED, TURNING_SPEED);
    motor_shield.change_motor_direction(TURN_RIGHT);
    turning = true;
}

inline void right_180()
{
    accelerometer.yaw_ = 180;
    rotation = accelerometer.get_yaw();
    maze.position.direction_step = (maze.position.direction_step + 2) % 4;
    maze.position.direction = dir_arr[maze.position.direction_step];
    motor_shield.set_speed(TURNING_SPEED, TURNING_SPEED, TURNING_SPEED, TURNING_SPEED);
    motor_shield.change_motor_direction(TURN_RIGHT);
    turning = true;
}

inline void left_180()
{
    accelerometer.yaw_ = 180;
    rotation = accelerometer.get_yaw();
    maze.position.direction_step = (maze.position.direction_step + 2) % 4;
    maze.position.direction = dir_arr[maze.position.direction_step];
    motor_shield.set_speed(TURNING_SPEED, TURNING_SPEED, TURNING_SPEED, TURNING_SPEED);
    motor_shield.change_motor_direction(TURN_LEFT);
    turning = true;
}

inline void reset_speed()
{
    stop();
    accelerometer.get_forwards_movement(); // just to reset everything
    accelerometer.get_forwards_movement(); // just to reset everything
    accelerometer.yaw_ = 0;
    rotation = accelerometer.get_yaw();
    motor_shield.change_motor_direction(GO_FORWARD);
    motor_shield.set_speed(STANDARD_FORWARD_SPEED, STANDARD_FORWARD_SPEED, STANDARD_FORWARD_SPEED, STANDARD_FORWARD_SPEED);
    turning = false; 
}

inline void strafe_left()
{
    accelerometer.yaw_ = 0;
    rotation = accelerometer.get_yaw();
    motor_shield.set_speed(STANDARD_FORWARD_SPEED - STRAFE_CONSTANT, STANDARD_FORWARD_SPEED, STANDARD_FORWARD_SPEED, STANDARD_FORWARD_SPEED - STRAFE_CONSTANT);
    delay(STRAFE_DELAY);
    turning = true;
}

inline void strafe_right()
{
    accelerometer.yaw_ = 0;
    rotation = accelerometer.get_yaw();
    motor_shield.set_speed(STANDARD_FORWARD_SPEED, STANDARD_FORWARD_SPEED - STRAFE_CONSTANT, STANDARD_FORWARD_SPEED - STRAFE_CONSTANT, STANDARD_FORWARD_SPEED);
    delay(STRAFE_DELAY);
    turning = true;
}

inline void stop()
{
    motor_shield.change_motor_direction(GO_BACK);
    accelerometer.forward_velocity = 0.0f;
    delay(50); // delete
    motor_shield.change_motor_direction(STOP);
}

inline void left(float angle) // THIS ONE SHOULD BE FIXED
{
    accelerometer.yaw_ = -1 * abs(angle);
    rotation = accelerometer.get_yaw();
    maze.position.direction_step = (maze.position.direction_step + 3) % 4;
    maze.position.direction = dir_arr[maze.position.direction_step];
    motor_shield.set_speed(TURNING_SPEED, TURNING_SPEED, TURNING_SPEED, TURNING_SPEED);
    motor_shield.change_motor_direction(TURN_LEFT);
    turning = true;
    
}

inline void right(float angle) // THIS ONE SHOULD BE FIXED, THE 
{
    accelerometer.yaw_ = abs(angle);
    rotation = accelerometer.get_yaw();
    maze.position.direction_step = (maze.position.direction_step + 5) % 4;
    maze.position.direction = dir_arr[maze.position.direction_step];
    motor_shield.set_speed(TURNING_SPEED, TURNING_SPEED, TURNING_SPEED, TURNING_SPEED);
    motor_shield.change_motor_direction(TURN_RIGHT);
    turning = true;
}
