//PINOUT:
//SDA = A4
//SCL = A5
#include "Arduino.h"
#include "OLED128x64.h"

#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"

// 0X3C+SA0 - 0x3C or 0x3D
#define I2C_ADDRESS 0x3C

SSD1306AsciiWire oled;

oledclass::oledclass(){}

int oledclass::SEND64(String Line1, String Line2, String Line3, String Line4, String Line5)
{
  Wire.begin();         
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
  oled.set400kHz();  
  oled.setFont(Adafruit5x7);  

  oled.clear();  
  oled.println(Line1);
  oled.set2X();
  oled.println(Line2);
  oled.println(Line3);
  oled.println(Line4);
  oled.set1X();
  oled.println(Line5);
  return(1);
}

oledclass OLED = oledclass();
