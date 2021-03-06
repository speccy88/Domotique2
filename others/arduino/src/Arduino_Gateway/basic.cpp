#include <avr/io.h>
#include "Arduino.h"
#include "defines.h"
#include "globals.h"
#include "command_template.h"
#include "basic.h"

#define DEBUG

Basic::Basic()
{
  name = "basic";
}

void Basic::Process()
{
  #ifdef DEBUG
    Serial.println("[PROCESS]");
  #endif
 
  int pin = atoi(command_ptr[1]);
  unsigned int level = atoi(command_ptr[2]);
  unsigned long duration = atoi(command_ptr[3]);
  int address = atoi(command_ptr[3]);
  int startstop = atoi(command_ptr[4]);

  #ifdef DEBUG
    Serial.print("Pin: ");
    Serial.println(pin);
    Serial.print("Level: ");
    Serial.println(level);
  #endif

  // FIRST COMPARISON ALWAYS FALSE - THIS ALLOWS TO DISABLE SUBROUTINES
  if(strcmp(command_ptr[0],"write") == 0)
    dtostrf(Set(pin, level), 1, 0, UDP_Reply_Buffer);
  if(strcmp(command_ptr[0],"status") == 0)
    dtostrf(Status(pin), 1, 0, UDP_Reply_Buffer);
  if(strcmp(command_ptr[0],"read") == 0)
    dtostrf(Read(pin), 1, 0, UDP_Reply_Buffer);
  if(strcmp(command_ptr[0],"analog") == 0)
    dtostrf(Analog(pin), 1, 0, UDP_Reply_Buffer);
  if(strcmp(command_ptr[0],"pwm") == 0)
    dtostrf(Pwm(pin, level), 1, 0, UDP_Reply_Buffer);
  if(strcmp(command_ptr[0],"freq") == 0)
    dtostrf(Freq(pin), 3, 1, UDP_Reply_Buffer);
  if(strcmp(command_ptr[0],"tone") == 0)
    dtostrf(Tone(pin, level, duration, startstop), 3, 1, UDP_Reply_Buffer);


  #ifdef DEBUG
    Serial.print("Return: ");
    Serial.println(UDP_Reply_Buffer);
  #endif
}

int Basic::Set(int pin, unsigned int level)
{
    Serial.println("[Write]");
  if((dig_lo_min <= pin <= dig_lo_max) || (dig_hi_min <= pin <= dig_hi_max))
  {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, level);

    return((int)digitalRead(pin) == level ? OPERATION_SUCCESSFUL : OPERATION_FAILED);
  }
  else
    return(ERROR_INVALID_DIGITAL_PIN);
}

int Basic::Read(int pin)
{
  if((dig_lo_min <= pin <= dig_lo_max) || (dig_hi_min <= pin <= dig_hi_max))
  {
    pinMode(pin, INPUT_PULLUP);
      
    return(digitalRead(pin) ? 1 : 0);
  }
  else
    return(ERROR_INVALID_DIGITAL_PIN);
}

int Basic::Status(int pin)
{
  if((dig_lo_min <= pin <= dig_lo_max) || (dig_hi_min <= pin <= dig_hi_max))
  {
    return(digitalRead(pin) ? 1 : 0);
  }
  else
    return(ERROR_INVALID_DIGITAL_PIN);
}

int Basic::Analog(int pin)
{
  if(pin <= analog_a0 && pin <= analog_a7)
    pin += 14;

  if(pin >= analog_min && pin <= analog_max)

  
    return(analogRead(pin));
  else
    return(ERROR_INVALID_ANALOGIN_PIN);
}

int Basic::Pwm(int pin, unsigned int level)
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

double Basic::Freq(int pin)
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

int Basic::Tone(int pin, unsigned int level, unsigned long duration, int startstop)
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
