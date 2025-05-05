#include <stdint.h>
#include <HardwareSerial.h>

class Accelerometer
{
private:
    unsigned long last_update_time_ ;
    HardwareSerial* serialPort2; // Pointer to Serial2 object
public:
    float yaw_ = 0;
    float forward_velocity = 0.0;
    float forward_position = 0.0;
    unsigned long last_time_movement = 0;
    int16_t get_yaw(void);
    float get_forwards_movement(void);
    void initializeSerial2(long baudRate);
    Accelerometer();
};
