#include "Arduino.h"
#include <EEPROM.h>
#include "digital.h"


digclass::digclass()
{
  //uint8_t vu;
}

int digclass::SET(int pin, int level)
{
  pinMode(pin, OUTPUT);
  digitalWrite(pin, level);

  return((int)digitalRead(pin));
}

int digclass::READ(int pin)
{
  pinMode(pin, INPUT);

  return((int)digitalRead(pin));
}

int digclass::STATUS(int pin)
{
  return((int)digitalRead(pin));
}

int digclass::ANALOG(int pin)
{
  return((int)analogRead(pin));
}

int digclass::PWM(int pin, int level)
{
  analogWrite(pin, level);
  return(level);
}

digclass digital = digclass();

