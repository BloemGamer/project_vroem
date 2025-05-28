#include "Arduino.h"
#include "bluetooth.h"

char Blue_Tooth::bluetooth_read_char()
{
    Serial1.begin(9600);
    if (Serial1.available())
    {
        char bluetooth_char = Serial1.read();
        if(bluetooth_char != '\0')
        {
          return bluetooth_char;
        }
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
            if(bluetooth_char == '\0')
            {
                bluetooth_string[i] = '\n';
                break;
            }
        }
        Serial.println(bluetooth_string);
    }

    return bluetooth_string;
}

void Blue_Tooth::bluetooth_send_int64(int64_t value) 
{
    for (uint8_t i = 0; i < 8; i++) 
    {
        Serial1.write((byte)((value >> (i * 8)) & 0xFF));
    }
}