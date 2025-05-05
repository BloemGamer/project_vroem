#include "LedControl.h"

class Led_Matrix
{
private:
    LedControl lc = LedControl(45,49,47,1);

public:
    Led_Matrix(void);
    void draw(byte* picture);
    void show_sensors(void);
};

