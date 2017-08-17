//PINOUT:
//SDA = A4
//SCL = A5
#include "Arduino.h"
#include "OLED.h"

#include "error_codes.h"                                                               // Includes error codes and pins definitions (min/max pin numbers)

#include <SSD1306_text.h>

#include <Wire.h>

// 0X3C+SA0 - 0x3C or 0x3D
#define I2C_ADDRESS 0x3C

#define OLED_RESET 22     // Reset pin or 22 for no reset pin
SSD1306_text display(OLED_RESET);

oledclass::oledclass(){}

int oledclass::SEND(int NbrLines, char Line1, char Line2, char Line3, char Line4, char Line5, char Line6, char Line7, char Line8)
{
  display.init(I2C_ADDRESS);

  if(NbrLines == 8)
  {
    display.clear();
    
    display.setTextSize(1, 1);
    display.setCursor(0, 0); //(y, x)
    display.write(Line1);
    display.setCursor(0, 1); //(y, x)
    display.write(Line2);
    display.setCursor(0, 2); //(y, x)
    display.write(Line3);
    display.setCursor(0, 3); //(y, x)
    display.write(Line4);
    display.setCursor(0, 4); //(y, x)
    display.write(Line5);
    display.setCursor(0, 5); //(y, x)
    display.write(Line6);
    display.setCursor(0, 6); //(y, x)
    display.write(Line7);
    display.setCursor(0, 7); //(y, x)
    display.write(Line8);
    return(OPERATION_SUCCESSFUL);
  }
  else if(NbrLines == 5)
  {
    display.clear(); 
    
    display.setTextSize(1, 1);
    display.setCursor(0, 0); //(y, x)
    display.write(Line1);
    display.setTextSize(2, 1);
    display.setCursor(0, 2); //(y, x)
    display.write(Line2);
    display.setCursor(0, 4); //(y, x)
    display.write(Line3);
    display.setCursor(0, 6); //(y, x)
    display.write(Line4);
    display.setTextSize(1, 1);
    display.setCursor(0, 7); //(y, x)
    display.write(Line5);
    return(OPERATION_SUCCESSFUL);
  }
  else if(NbrLines == 2)
  {
    display.clear();
    
    display.setTextSize(1, 1);
    display.setCursor(0, 0); //(y, x)
    display.write(Line1);
    display.setCursor(0, 3); //(y, x)
    display.setTextSize(4, 1);
    display.write(Line2);
    return(OPERATION_SUCCESSFUL);
  }
  else
    return(ERROR_COMMAND_NOT_ACTIVATED);
}

oledclass OLED = oledclass();
