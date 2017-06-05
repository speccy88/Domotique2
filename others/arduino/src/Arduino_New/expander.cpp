#include "Arduino.h"
#include "expander.h"

#include <Wire.h>    // Required for I2C communication
#include "PCF8575.h" // Required for PCF8575

PCF8575 pcf8575;

int hex;

gpioclass::gpioclass(){}

int gpioclass::SET(int pin, int level, int address)
{
  hex = address + 32;
  pcf8575.begin(hex); //0x20 to 0x27
  if(0 <= pin <= 15)                                         //Pin validation
  {
    pcf8575.pinMode(pin, OUTPUT);
    pcf8575.digitalWrite(pin, level);
    return((int)pcf8575.digitalRead(pin));
  }
  else
    return(9997);                                            //Reply "9997" means the pin number is not valid for digital read/write
}

int gpioclass::READ(int pin, int address)
{
  hex = address + 32;
  pcf8575.begin(0x20); //0x20 to 0x27
  if(0 <= pin <= 15)                                         //Pin validation
  {
    pcf8575.pinMode(pin, INPUT);
    return((int)pcf8575.digitalRead(pin));
  }
  else
    return(9997);                                            //Reply "9997" means the pin number is not valid for digital read/write
}

int gpioclass::STATUS(int pin, int address)
{
  hex = address + 32;
  pcf8575.begin(0x20); //0x20 to 0x27
  if(0 <= pin <= 15)                                         //Pin validation
  {
    return((int)pcf8575.digitalRead(pin));
  }
  else
    return(9997);                                            //Reply "9997" means the pin number is not valid for digital read/write
}

gpioclass gpio = gpioclass();

