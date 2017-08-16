#include "Arduino.h"
#include "digital.h"
#include "temp.h"
#include "pressure.h"
#include "expander.h"
#include "stgc.h"
#include "OLED.h"

#include "error_codes.h"                                                               // Includes error codes and pins definitions (min/max pin numbers)
#include "ip_address.h"

digclass* digital;

digclass::digclass(char (*commands_ptr)[COMMAND_LENGTH])
{
  commands = commands_ptr;
}

void digclass::process(char* return_str)
{
  Serial.println("PROCESS CLASS");
  int pin = atoi(commands[1]);
  int level = atoi(commands[2]);
  char units = commands[2];
  int duration = atoi(commands[3]);
  int address = atoi(commands[3]);
  int startstop = atoi(commands[4]);

  // FIRST COMPARISON ALWAYS FALSE - THIS WILL ALLOW TO DISABLE SOME SUBROUTINES
  if(false)
    delayMicroseconds(0);
  #ifdef enable_digital
    else if(strcmp(commands[0],"write") == 0)
      sprintf(return_str, "%d", SET(pin, level));
    else if(strcmp(commands[0],"read") == 0)
      sprintf(return_str, "%d", READ(pin));
    else if(strcmp(commands[0],"status") == 0)
      sprintf(return_str, "%d", STATUS(pin));
  #else
    else if(strcmp(commands[0],"write") == 0 || strcmp(commands[0],"read" == 0) || strcmp(commands[0],"status" == 0))
      sprintf(return_str, "%d", ERROR_COMMAND_NOT_ACTIVATED);
  #endif

  //ANALOG INPUTS - READING ANALOG INPUT PIN
  #ifdef enable_analogin
    else if(strcmp(commands[0],"analog") == 0)
      sprintf(return_str, "%d", ANALOG(pin));
  #else
    else if(strcmp(commands[0],"analog") == 0)
      sprintf(return_str, "%d", ERROR_COMMAND_NOT_ACTIVATED);
  #endif

  // ANALOG OUTPUTS (PWM) - WRITE ANALOG OUTPUT PIN (PWM)
  #ifdef enable_analogout
    else if(strcmp(commands[0],"pwm") == 0)
      sprintf(return_str, "%d", PWM(pin, level));
  #else
    else if(strcmp(commands[0],"pwm") == 0)
      sprintf(return_str, "%d", ERROR_COMMAND_NOT_ACTIVATED);
  #endif
  
  // FREQUENCY COUNTER - READ PIN FREQUENCY in Hz
  #ifdef enable_frequency    
    else if(strcmp(commands[0],"freq") == 0)
      sprintf(return_str, "%d", FREQ(pin));
  #else
    else if(strcmp(commands[0],"freq") == 0)
      sprintf(return_str, "%d", ERROR_COMMAND_NOT_ACTIVATED);
  #endif

  // TONE GENERATOR - MAKE TONE ON DIGITAL PIN
  #ifdef enable_tone
    else if(strcmp(commands[0],"tone") == 0)
      sprintf(return_str, "%d", TONE(pin, level, duration, startstop));
  #else
    else if(strcmp(commands[0],"tone") == 0)
      sprintf(return_str, "%d", ERROR_COMMAND_NOT_ACTIVATED);
  #endif

  // GPIO EXPANDER
  // GPIO EXPANDER BASED ON PCF8575 (or PCF8574 with some modifications)
  #ifdef enable_expander
    else if(strcmp(commands[0],"expwrite") == 0)
      sprintf(return_str, "%d", expander.SET(pin, level, address));
    else if(strcmp(commands[0],"expread") == 0)
      sprintf(return_str, "%d", expander.READ(pin, address));
  #else
    else if(strcmp(commands[0],"expwrite") == 0 || strcmp(commands[0],"expread") == 0)
      sprintf(return_str, "%d", ERROR_COMMAND_NOT_ACTIVATED);
  #endif
  
  // TEMPERATURE/HUMIDITY
  // READ TEMPERATURE, HUMIDITY AND HEAT INDEX USING DHT22
  // commands[2] should be 0 for 
  #ifdef enable_temp
    else if(strcmp(commands[0],"temp") == 0)
      sprintf(return_str, "%d,%d,%d", temp.TEMPERATURE(pin, units, 1), temp.TEMPERATURE(pin, units, 2), temp.TEMPERATURE(pin, units, 3));
  #else
    else if(strcmp(commands[0],"temp") == 0)
      sprintf(return_str, "%d", ERROR_COMMAND_NOT_ACTIVATED);
  #endif

  // BAROMETRIC PRESSURE
  // READ BAROMETRIC PRESSURE USING BMP280
  #ifdef enable_baro
    else if(strcmp(commands[0],"pres") == 0)
      sprintf(return_str, "%d", pressure.PRES(pin));
  #else
    else if(strcmp(commands[0],"pres") == 0)
      sprintf(return_str, "%d", ERROR_COMMAND_NOT_ACTIVATED);
  #endif

  // STGC DECODER
  // READ SINGLE TRACK GRAY CODE ENCODER
  #ifdef enable_stgc
    else if(strcmp(commands[0],"stgc") == 0)
      sprintf(return_str, "%d", STGC.READ(atoi(commands[1]), atoi(commands[2]), atoi(commands[3]), atoi(commands[4]), atoi(commands[5])));
  #else
    else if(strcmp(commands[0],"stgc") == 0)
      sprintf(return_str, "%d", ERROR_COMMAND_NOT_ACTIVATED);
  #endif

  // OLED DISPLAY WITH ASCII ONLY LIBRARIES
  // This function is predefined to 2, 5 or 8 lines:
  // OLED DISPLAY USING SSD1306 128x64 (maybe 32 later)
  
  #ifdef enable_oled
    else if(strcmp(commands[0],"oled") == 0)
      sprintf(return_str, "%d", OLED.SEND(commands[1], commands[2], commands[3], commands[4], commands[5], commands[6], commands[7], commands[8], commands[9]));
  #else
    else if(strcmp(commands[0],"oled") == 0)
      sprintf(return_str, "%d", ERROR_COMMAND_NOT_ACTIVATED);
  #endif

  else
  {
    Serial.println("FAIL TO FIND FUNCTION");
    sprintf(return_str, "%d", ERROR_UNDEFINED_COMMAND);
  } 
}

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
    pinMode(pin, INPUT_PULLUP);
    int level = (int)digitalRead(pin);
      
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
  if(pin <= analog_a0 && pin <= analog_a7)
    pin += 14;

  if(pin >= analog_min && pin <= analog_max)
    return((int)analogRead(pin));
  else
    return(ERROR_INVALID_ANALOGIN_PIN);
}

int digclass::PWM(int pin, int level)
{
  // Convert data from 0-100% to 0-255
  level = constrain(level, 0, 100);
  level = map(level, 0, 100, 0, 255);

  if(pin == pwm_3 || pin == pwm_5 || pin == pwm_6 || pin == pwm_9)
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

int digclass::TONE(int pin, int level, int duration, int startstop)
{
  if((dig_lo_min <= pin <= dig_lo_max) || (dig_hi_min <= pin <= dig_hi_max))
  {
    if(startstop == 1)
    {
      if(31 >= duration <= 65535)
      {
        tone(pin, level, duration);
        return(level);
      }
      else
        return(0);                                           //Reply "0" means the pin tone is OFF or out of range
    }
  
    if(startstop == 0)
    {
      noTone(pin);
      return(0);                                             //Reply "0" means the pin tone is OFF or out of range
    }
  }
  else
    return(ERROR_INVALID_DIGITAL_PIN);
}
