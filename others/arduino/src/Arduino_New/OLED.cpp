//PINOUT:
//SDA = A4
//SCL = A5
#include "Arduino.h"
#include "OLED.h"

#include "error_codes.h"                                                               // Includes error codes and pins definitions (min/max pin numbers)

#define enable_oled_large   //16% memory use

#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"

// 0X3C+SA0 - 0x3C or 0x3D
#define I2C_ADDRESS 0x3C

SSD1306AsciiWire oled;

oledclass::oledclass(){}

int oledclass::SEND(int NbrLines, String Line1, String Line2, String Line3, String Line4, String Line5, String Line6, String Line7, String Line8)
{
  Wire.begin();         
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
  oled.set400kHz();  

  if(NbrLines == 0)
  {
    oled.setContrast(Line1.toInt());
    return(1);
  }

  if(NbrLines == 8)
  {
    oled.setContrast(Line8.toInt());
    oled.setFont(Adafruit5x7);  
    oled.set1X();
    oled.clear();  
    oled.println(Line1);
    oled.println(Line2);
    oled.println(Line3);
    oled.println(Line4);
    oled.println(Line5);
    oled.println(Line6);
    oled.println(Line7);
    oled.println(Line8);
    return(1);
  }

  if(NbrLines == 5)
  {
    oled.setFont(Adafruit5x7);  
    oled.set1X();
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

  #ifdef enable_oled_large
    if(NbrLines == 2)
    {
      oled.setFont(Adafruit5x7); 
      oled.clear();
      oled.set1X();
      oled.println(Line1);
      oled.setFont(CalLite24);    
      oled.set2X();
      oled.println(Line2);
      return(1);
    }
  #else
    if((NbrLines == 2))
      return(ERROR_COMMAND_NOT_ACTIVATED);
  #endif
  
}

oledclass OLED = oledclass();
