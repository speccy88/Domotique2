#include "Arduino.h"
#include "digital.h"

digclass::digclass(){}

int digclass::SET(int pin, int level)
{
  if((3 <= pin <= 9) || (13 <= pin <= 19))                   //Discard SPI pins (2(Unknown), 10(CS), 11(MOSI), 12(MISO), 13(SCK)) - Also discard pins that are analog exclusive (20(A6), 21(A7))
  {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, level);
    return((int)digitalRead(pin));
  }
  else
    return(9997);                                            //Reply "9997" means the pin number is not valid for digital read/write
}

int digclass::READ(int pin)
{
  if((3 <= pin <= 9) || (13 <= pin <= 19))                   //Discard SPI pins (2(Unknown), 10(CS), 11(MOSI), 12(MISO), 13(SCK)) - Also discard pins that are analog exclusive (20(A6), 21(A7))
  {
    pinMode(pin, INPUT);
    return((int)digitalRead(pin));
  }
  else
    return(9997);                                            //Reply "9997" means the pin number is not valid for digital read/write
}

int digclass::STATUS(int pin)
{
  if((3 <= pin <= 9) || (13 <= pin <= 19))                   //Discard SPI pins (2(Unknown), 10(CS), 11(MOSI), 12(MISO), 13(SCK)) - Also discard pins that are analog exclusive (20(A6), 21(A7))
  {
    return((int)digitalRead(pin));
  }
  else
    return(9997);                                            //Reply "9997" means the pin number is not valid for digital read/write
}

int digclass::ANALOG(int pin)
{
  if(0 <= pin <= 7)
    pin += 14;

  if(14 <= pin <= 21)
    return((int)analogRead(pin));
  else
    return(9996);                                            //Reply "9996" means the pin number is not valid for analog input read
}

int digclass::PWM(int pin, int level)
{
  // Convert data from 0-100% to 0-255
  level /= 100;

  if(level > 255)
    level = 255;
  else if(level < 0)
    level = 0;

  if(pin == (3 || 5 || 6 || 9))
  {
    analogWrite(pin, level);
    return(level);
  }
  else
    return(9993);                                            //Reply "9993" means the pin number is not valid for analog output write (PWM)
}

int digclass::FREQ(int pin)
{
  int Htime;                                                 //Integer for storing high time
  int Ltime;                                                 //Integer for storing low time
  int frequency;                                             //Storing frequency (float???)
  int TotalTime;                                             //Storing total cycle time (float???)

  if((3 <= pin <= 9) || (13 <= pin <= 19))                   //Discard SPI pins (2(Unknown), 10(CS), 11(MOSI), 12(MISO), 13(SCK)) - Also discard pins that are analog exclusive (20(A6), 21(A7))
  {
    pinMode(pin,INPUT);
    Htime = pulseIn(pin,HIGH);                                 //Read high time in microseconds (timeout default = 1s)
    Ltime = pulseIn(pin,LOW);                                  //Read low time in microseconds (timeout default = 1s)

    TotalTime = Htime+Ltime;

    if(TotalTime > 0)
    {
      frequency = (1000000/(Htime+Ltime))*10;                     //Calculate frequency * 10
      return(frequency);
    }
    else
      return(0);                                             //Reply "0" means the input frequency is very low or none
    

    
  }
  else
    return(9997);                                            //Reply "9997" means the pin number is not valid for digital read/write
}

int digclass::TONE(int pin, int level, int duration, String startstop)
{
  if((3 <= pin <= 9) || (13 <= pin <= 19))                   //Discard SPI pins (2(Unknown), 10(CS), 11(MOSI), 12(MISO), 13(SCK)) - Also discard pins that are analog exclusive (20(A6), 21(A7))
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
    return(9997);                                            //Reply "9997" means the pin number is not valid for digital read/write
}

digclass digital = digclass();

