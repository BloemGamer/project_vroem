#include "Arduino.h"
#include "bluetooth.h"

char Blue_Tooth::bluetoothRead()
{
  Serial1.begin(9600);
  if (Serial1.available())
  {
    char bluetoothChar = Serial1.read();
//    Serial.println(bluetoothChar);
    return bluetoothChar;
  }
  return '\0';
}
