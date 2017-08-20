//PINOUT:
//SDA = A4
//SCL = A5
#include "Arduino.h"
#include "expander.h"

#include "error_codes.h"                                                               // Includes error codes and pins definitions (min/max pin numbers)

#include <Wire.h>                                                                      // Required for I2C communication
#include "PCF8575.h"                                                                   // Required for PCF8575

PCF8575 pcf8575;

gpioclass::gpioclass(){}

int gpioclass::SET(int pin, unsigned int level, int address)
{
  int hex = address + 32;//0x20 to 0x27
  pcf8575.begin(hex); //0x20 to 0x27
  level = level == 1 ? 0 : 1; //Invert logic since output is reversed (PCF8575C are used)
  
  if((exp_lo_min <= pin <= exp_lo_max) || (exp_hi_min <= pin <= exp_hi_max))           //Pin validation
  {
    pcf8575.pinMode(pin, OUTPUT);
    pcf8575.digitalWrite(pin, level);
    return((int)pcf8575.digitalRead(pin) == level ? OPERATION_SUCCESSFUL : OPERATION_FAILED);
  }
  else
    return(ERROR_INVALID_DIGITAL_PIN);
}

int gpioclass::READ(int pin, int address)
{
  int hex = address + 32;
  pcf8575.begin(0x20); //0x20 to 0x27
  if((exp_lo_min <= pin <= exp_lo_max) || (exp_hi_min <= pin <= exp_hi_max))           //Pin validation
  {
    pcf8575.pinMode(pin, INPUT_PULLUP);
    return((int)pcf8575.digitalRead(pin));
  }
  else
    return(ERROR_INVALID_DIGITAL_PIN);
}

gpioclass gpio = gpioclass();

