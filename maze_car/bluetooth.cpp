#include "Arduino.h"
#include "bluetooth.h"

char Blue_Tooth::bluetooth_read_char()
{
    Serial1.begin(9600);
    if (Serial1.available())
    {
        char bluetooth_char = Serial1.read();
        return bluetooth_char;
    }
    else
    {
        return '\0';
    }
}

const char* Blue_Tooth::bluetooth_read_string()
{
    // Serial1.begin(9600);
    if (Serial1.available())
    {
        char bluetooth_char;
        for(int i = 0; i < 99; i++)
        {
            do
            {
                bluetooth_char = Serial1.read();
            }while(bluetooth_char == -1);
            // Serial.print((int)bluetooth_char);
            bluetooth_string[i] = bluetooth_char;
            if(bluetooth_char == '\n')
            {
                bluetooth_string[i] = '\0';
                break;
            }
        }
        Serial.println(bluetooth_string);
    }

    return bluetooth_string;
}
