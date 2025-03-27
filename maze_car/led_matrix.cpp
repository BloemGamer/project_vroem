#include "Arduino.h"
#include "led_matrix.h"
/*
 DIN pin
 CLK pin
 CS pin
*/

// delay time between faces
unsigned long delaytime=1000;

// happy face
byte hf[8]= { 0b00111100, 0b01000010, 0b10100101, 0b10000001, 0b10100101, 0b10011001, 0b01000010, 0b00111100 };
// neutral face
byte nf[8]={ 0b00111100,  0b01000010, 0b10100101, 0b10000001, 0b10111101, 0b10000001, 0b01000010, 0b00111100 };
// sad face
byte sf[8]= { 0b00111100, 0b01000010, 0b10100101, 0b10000001, 0b10011001, 0b10100101, 0b01000010, 0b00111100 };

Led_Matrix::led_matrix(void)
{
  lc.shutdown(0,false);
  // Set brightness to a medium value
  lc.setIntensity(0,8);
  // Clear the display
  lc.clearDisplay(0);  
}

void Led_Matrix::draw(byte* picture)
{
  // Display sad face
  lc.setRow(0,0,picture[0]);
  lc.setRow(0,1,picture[1]);
  lc.setRow(0,2,picture[2]);
  lc.setRow(0,3,picture[3]);
  lc.setRow(0,4,picture[4]);
  lc.setRow(0,5,picture[5]);
  lc.setRow(0,6,picture[6]);
  lc.setRow(0,7,picture[7]);
}

