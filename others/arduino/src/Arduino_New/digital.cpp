#include "Arduino.h"
#include "digital.h"

#include "error_codes.h"                                                               // Includes error codes and pins definitions (min/max pin numbers)

digclass::digclass(){}

int digclass::SET(int pin, int level)
{
  if((dig_lo_min <= pin <= dig_lo_max) || (dig_hi_min <= pin <= dig_hi_max))
  {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, level);
    return((int)digitalRead(pin));
  }
  else
    return(ERROR_INVALID_DIGITAL_PIN);
}

int digclass::READ(int pin)
{
  if((dig_lo_min <= pin <= dig_lo_max) || (dig_hi_min <= pin <= dig_hi_max))
  {
    pinMode(pin, INPUT);
    return((int)digitalRead(pin));
  }
  else
    return(ERROR_INVALID_DIGITAL_PIN);
}

int digclass::STATUS(int pin)
{
  if((dig_lo_min <= pin <= dig_lo_max) || (dig_hi_min <= pin <= dig_hi_max))
    return((int)digitalRead(pin));
  else
    return(ERROR_INVALID_DIGITAL_PIN);
}

int digclass::ANALOG(int pin)
{
  if(analog_a0 <= pin <= analog_a7)
    pin += 14;

  if(analog_min <= pin <= analog_max)
    return((int)analogRead(pin));
  else
    return(ERROR_INVALID_ANALOGIN_PIN);
}

int digclass::PWM(int pin, int level)
{
  // Convert data from 0-100% to 0-255
  level /= 100;
  level *= 255;

  if(level > 255)
    level = 255;
  else if(level < 0)
    level = 0;

  if(pin == (pwm_3 || pwm_5 || pwm_6 || pwm_9))
  {
    analogWrite(pin, level);
    return(level);
  }
  else
    return(ERROR_INVALID_PWM_PIN);
}

int digclass::FREQ(int pin)
{
  int Htime;                                                 //Integer for storing high time
  int Ltime;                                                 //Integer for storing low time
  int frequency;                                             //Storing frequency (float???)
  int TotalTime;                                             //Storing total cycle time (float???)

  if((dig_lo_min <= pin <= dig_lo_max) || (dig_hi_min <= pin <= dig_hi_max))
  {
    pinMode(pin,INPUT);
    Htime = pulseIn(pin,HIGH);                               //Read high time in microseconds (timeout default = 1s)
    Ltime = pulseIn(pin,LOW);                                //Read low time in microseconds (timeout default = 1s)

    TotalTime = Htime+Ltime;

    if(TotalTime > 0)
    {
      frequency = (1000000/(Htime+Ltime))*10;                //Calculate frequency * 10
      return(frequency);
    }
    else
      return(0);                                             //Reply "0" means the input frequency is very low or none
  }
  else
    return(ERROR_INVALID_DIGITAL_PIN);
}

int digclass::TONE(int pin, int level, int duration, String startstop)
{
  if((dig_lo_min <= pin <= dig_lo_max) || (dig_hi_min <= pin <= dig_hi_max))
  {
    if(startstop == "start")
    {
      if(31 >= duration <= 65535)
      {
        tone(pin, level, duration);
        return(level);
      }
      else
        return(0);                                           //Reply "0" means the pin tone is OFF or out of range
    }
  
    if(startstop == "stop")
    {
      noTone(pin);
      return(0);                                             //Reply "0" means the pin tone is OFF or out of range
    }
  }
  else
    return(ERROR_INVALID_DIGITAL_PIN);
}

digclass digital = digclass();

