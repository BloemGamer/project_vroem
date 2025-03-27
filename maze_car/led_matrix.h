#include "LedControl.h"

class led_matrix
{
private:
  LedControl lc=LedControl(45,47,49,1);

public:
  led_matrix(void);
  void draw(byte* picture);
};

