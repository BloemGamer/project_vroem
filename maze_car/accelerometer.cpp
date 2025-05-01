#include "Accelerometer.h"
#include <Wire.h>
#include "I2Cdev.h"
#include "MPU6050.h"

MPU6050 imu;

Accelerometer::Accelerometer()
{
  Wire.begin();
  //imu.initialize();
  last_update_time_ = millis();
}

void Accelerometer::initializeSerial2(long baudRate) {
    Serial2.begin(baudRate);
    serialPort2 = &Serial2; // Assign the Serial2 object to the pointer
    Serial.print("Serial2 initialized at ");
    Serial.print(baudRate);
    Serial.println(" bps");
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
    return static_cast<int16_t>(yaw_);
}

int8_t Accelerometer::get_forwards_movement(void)
{
    return 0;
}