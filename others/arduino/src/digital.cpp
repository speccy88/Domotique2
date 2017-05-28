#include "Arduino.h"
#include <EEPROM.h>
#include "digital.h"


digclass::digclass()
{
  //uint8_t vu;
}

int digclass::SET(int pin, bool level)
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

digclass digital = digclass();

