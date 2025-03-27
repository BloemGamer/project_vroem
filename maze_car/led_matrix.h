#include "LedControl.h"

class Led_Matrix
{
private:
  LedControl lc=LedControl(45,47,49,1);

public:
  Led_Matrix(void);
  void draw(byte* picture);
};

