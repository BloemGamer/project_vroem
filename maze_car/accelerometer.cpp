#include "accelerometer.h"
#include <Wire.h>

// just so I don't get warnings
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#include "I2Cdev.h"
#include "MPU6050.h"
#pragma GCC diagnostic pop

MPU6050 imu;
const float acceleration_scale = 1.0 / 16384.0; // Assuming +/- 2g range
const float alpha = 0.98; // Complementary filter weight for accelerometer
float gravity[3] = {0.0, 0.0, 1.0}; // Initial gravity vector


Accelerometer::Accelerometer()
{
    Wire.begin();
    //imu.initialize(); idk why but this needs to be commented
    last_update_time_ = millis();
}

void Accelerometer::initializeSerial2(long baudRate)
{
    Serial2.begin(baudRate);
    serialPort2 = &Serial2; // Assign the Serial2 object to the pointer
    /*
    Serial.print("Serial2 initialized at ");
    Serial.print(baudRate);
    Serial.println(" bps");
    */
}

int16_t Accelerometer::get_yaw(void)
{
    int16_t gx, gy, gz;
    imu.getRotation(&gx, &gy, &gz);
    unsigned long currentTime = millis();
    unsigned long deltaTime = currentTime - last_update_time_;
    last_update_time_ = currentTime;
    float gyroScaleFactor = 131.0;
    float yawRate = (float)gz / gyroScaleFactor;
    yaw_ += yawRate * (deltaTime / 1000.0);
    if(yaw_ > 180) yaw_ -= 360;
    if(yaw_ < -180) yaw_ += 360;
    return static_cast<int16_t>(yaw_);
}

float Accelerometer::get_forwards_movement(void)
{
    int16_t ax_raw, ay_raw, az_raw, gx, gy, gz;
    imu.getMotion6(&ax_raw, &ay_raw, &az_raw, &gx, &gy, &gz);

    unsigned long current_time = millis();
    unsigned long delta_time_ms = current_time - last_time_movement;
    last_time_movement = current_time;
    float delta_time_s = delta_time_ms / 1000.0;

    // Convert raw accelerometer readings to g-force
    float ax = ax_raw * acceleration_scale;
    float ay = ay_raw * acceleration_scale;
    float az = az_raw * acceleration_scale;

    // Simple Complementary Filter to estimate orientation (Roll and Pitch)
    gravity[0] = alpha * gravity[0] + (1 - alpha) * ax;
    gravity[1] = alpha * gravity[1] + (1 - alpha) * ay;
    gravity[2] = alpha * gravity[2] + (1 - alpha) * az;

    float roll = atan2(gravity[1], sqrt(gravity[0] * gravity[0] + gravity[2] * gravity[2]));
    float pitch = atan2(-gravity[0], sqrt(gravity[1] * gravity[1] + gravity[2] * gravity[2]));

    // Remove gravity component from accelerometer readings along an assumed forward axis
    // Assuming the sensor's X-axis is roughly aligned with the forward direction
    float forward_acceleration = ax - sin(pitch); // Project gravity out

    // Simple integration (prone to drift)
    forward_velocity += forward_acceleration * delta_time_s;
    forward_position += forward_velocity * delta_time_s;
    return forward_position;
    //float distance_cm = (forward_position);
    //return static_cast<int8_t>(distance_cm); // Return as int8_t (can lose precision)
}
