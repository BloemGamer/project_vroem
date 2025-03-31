#include "Arduino.h"
#include "led_matrix.h"
/*
 DIN pin
 CLK pin
 CS pin
*/

extern bool ir_right_trigged;
extern bool ir_left_trigged;
extern unsigned int measured_ultrasonic_distance_left, measured_ultrasonic_distance_right, measured_ultrasonic_distance_front;
void take_measurements(void);

// delay time between faces
// unsigned long delaytime=1000;

// happy face
// byte hf[8]= { 0b00111100, 0b01000010, 0b10100101, 0b10000001, 0b10100101, 0b10011001, 0b01000010, 0b00111100 };
// // neutral face
// byte nf[8]={ 0b00111100,  0b01000010, 0b10100101, 0b10000001, 0b10111101, 0b10000001, 0b01000010, 0b00111100 };
// // sad face
// byte sf[8]= { 0b00111100, 0b01000010, 0b10100101, 0b10000001, 0b10011001, 0b10100101, 0b01000010, 0b00111100 };

Led_Matrix::Led_Matrix(void)
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

void Led_Matrix::show_sensors(void)
{
    
byte picture[8] = { 0 };
  take_measurements();

  // ultrasoon sensors
  if(measured_ultrasonic_distance_front > 10)
  {
    picture[3] |= 0b00011000;
    if(measured_ultrasonic_distance_front > 20)
    {
      picture[2] |= 0b00011000;
      if(measured_ultrasonic_distance_front > 30)
      {
        picture[1] |= 0b00011000;
        if(measured_ultrasonic_distance_right > 40)
        {
          picture[0] |= 0b00011000;
        }
      }
    }
  }
  if(measured_ultrasonic_distance_left > 10)
  {
    picture[3] |= 0b00010000;
    picture[4] |= 0b00010000;
    if(measured_ultrasonic_distance_left > 20)
    {
      picture[3] |= 0b00100000;
      picture[4] |= 0b00100000;
      if(measured_ultrasonic_distance_left > 30)
      {
        picture[3] |= 0b01000000;
        picture[4] |= 0b01000000;
        if(measured_ultrasonic_distance_left > 40)
        {
          picture[3] |= 0b10000000;
          picture[4] |= 0b10000000;
        }
      }
    }
  }
  if(measured_ultrasonic_distance_right > 10)
  {
    picture[3] |= 0b00001000;
    picture[4] |= 0b00001000;
    if(measured_ultrasonic_distance_right > 20)
    {
      picture[3] |= 0b00000100;
      picture[4] |= 0b00000100;
      if(measured_ultrasonic_distance_right > 30)
      {
        picture[3] |= 0b00000010;
        picture[4] |= 0b00000010;
        if(measured_ultrasonic_distance_right > 40)
        {
          picture[3] |= 0b00000001;
          picture[4] |= 0b00000001;
        }
      }
    }
  }

  if(ir_left_trigged)
  {
    picture[0] |= 0b11000000;
    picture[1] |= 0b10000000;
  }
  if(ir_right_trigged)
  {
    picture[0] |= 0b00000011;
    picture[1] |= 0b00000001;
  }




  draw(picture);


}
