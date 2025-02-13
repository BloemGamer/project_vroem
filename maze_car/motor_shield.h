#include <stdint.h>

class Motor_Shield
{
private:
  uint8_t speed_motors[4];

public:
  void set_speed(uint8_t speed_m1, uint8_t speed_m2, uint8_t speed_m3, uint8_t speed_m4);
  void set_speed(uint8_t motor, uint8_t speed_m); // overloaden voor 1 motor aanpassen, en ze alle 4 tergelijkertijd aanpassen
  void update_speed();

  Motor_Shield();
  ~Motor_Shield();
};