#include "Arduino.h"
#include "base.h"

#include "error_codes.h"                                                               // Includes error codes and pins definitions (min/max pin numbers)

baseclass::baseclass(){}

int baseclass::SET(int pin, unsigned int level)
{
  if((dig_lo_min <= pin <= dig_lo_max) || (dig_hi_min <= pin <= dig_hi_max))
  {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, level);

    return((int)digitalRead(pin) == level ? OPERATION_SUCCESSFUL : OPERATION_FAILED);
  }
  else
    return(ERROR_INVALID_DIGITAL_PIN);
}

int baseclass::READ(int pin)
{
  if((dig_lo_min <= pin <= dig_lo_max) || (dig_hi_min <= pin <= dig_hi_max))
  {
    pinMode(pin, INPUT_PULLUP);
      
    return(digitalRead(pin) ? 1 : 0);
  }
  else
    return(ERROR_INVALID_DIGITAL_PIN);
}

int baseclass::ANALOG(int pin)
{
  if(pin <= analog_a0 && pin <= analog_a7)
    pin += 14;

  if(pin >= analog_min && pin <= analog_max)

  
    return(analogRead(pin));
  else
    return(ERROR_INVALID_ANALOGIN_PIN);
}

int baseclass::PWM(int pin, unsigned int level)
{
  // Convert data from 0-100% to 0-255
  level = constrain(level, 0, 100);
  level = map(level, 0, 100, 0, 255);

  if(pin == pwm_3 || pin == pwm_5 || pin == pwm_6 || pin == pwm_9)
  {
    analogWrite(pin, level);
    return(OPERATION_SUCCESSFUL);
  }
  else
    return(ERROR_INVALID_PWM_PIN);
}

double baseclass::FREQ(int pin)
{
  double Htime = 0;                                          //Integer for storing high time
  double Ltime = 0;                                          //Integer for storing low time
  double Ttime = 0;                                          //Storing total cycle time (float???)
  double frequency = 0;                                      //Storing frequency (float???)

  if((dig_lo_min <= pin <= dig_lo_max) || (dig_hi_min <= pin <= dig_hi_max))
  {
    pinMode(pin,INPUT);
    Htime = pulseIn(pin,HIGH);                               //Read high time in microseconds (timeout default = 1s)
    Ltime = pulseIn(pin,LOW);                                //Read low time in microseconds (timeout default = 1s)

    Ttime = Htime+Ltime;

    frequency = Ttime > 0 ? 1000000/Ttime : 0;               //Calculate frequency in Hz            

    return(frequency);
  }
  else
    return(ERROR_INVALID_DIGITAL_PIN);
}

int baseclass::TONE(int pin, unsigned int level, unsigned long duration, int startstop)
{
  if((dig_lo_min <= pin <= dig_lo_max) || (dig_hi_min <= pin <= dig_hi_max))
  {
    if(startstop == 1 && duration >= 31 && duration <= 65535)
    {
      tone(pin, level, duration);
      return(OPERATION_SUCCESSFUL);
    }
    else
    {
      noTone(pin);
      return(0);                                             //Reply "0" means the pin tone is OFF or out of range
    }
  }
  else
    return(ERROR_INVALID_DIGITAL_PIN);
}

baseclass base = baseclass();
