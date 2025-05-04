#include <stdint.h>
#include <HardwareSerial.h>

class Accelerometer
{
private:
  float yaw_ = 0;
  unsigned long last_update_time_ ;
  HardwareSerial* serialPort2; // Pointer to Serial2 object
public:
  int16_t get_yaw(void);
  float get_forwards_movement(void);
  void initializeSerial2(long baudRate);
  Accelerometer();
};