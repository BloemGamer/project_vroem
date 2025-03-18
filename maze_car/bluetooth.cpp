#include "Arduino.h"
#include "bluetooth.h"

void Blue_Tooth::bluetoothRead()
{
  Serial1.begin(9600);
  if (Serial1.available())
  {
    char bluetoothChar = Serial1.read();
    Serial.println(bluetoothChar);
  }
}
