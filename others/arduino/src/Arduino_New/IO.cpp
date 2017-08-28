#include "Arduino.h"
#include "io.h"
#include "base.h"
#include "temp.h"
#include "baro.h"
#include "expander.h"
#include "stgc.h"
#include "OLED.h"

#include "error_codes.h"                                                               // Includes error codes and pins definitions (min/max pin numbers)
#include "ip_address.h"

#include <avr/io.h>

ioclass* io;

ioclass::ioclass(char (*commands_ptr)[COMMAND_LENGTH])
{
  commands = commands_ptr;
}

void ioclass::process(char* return_str)
{
  int pin = atoi(commands[1]);
  unsigned int level = atoi(commands[2]);
  unsigned long duration = atoi(commands[3]);
  int address = atoi(commands[3]);
  int startstop = atoi(commands[4]);

  // FIRST COMPARISON ALWAYS FALSE - THIS ALLOWS TO DISABLE SUBROUTINES
  if(false)
    delayMicroseconds(0);
  #ifdef enable_base
    else if(strcmp(commands[0],"write") == 0)
      dtostrf(base.SET(pin, level), 1, 0, return_str);
    else if(strcmp(commands[0],"read") == 0)
      dtostrf(base.READ(pin), 1, 0, return_str);
    else if(strcmp(commands[0],"analog") == 0)
      dtostrf(base.ANALOG(pin), 1, 0, return_str);
    else if(strcmp(commands[0],"pwm") == 0)
      dtostrf(base.PWM(pin, level), 1, 0, return_str);
    else if(strcmp(commands[0],"freq") == 0)
      dtostrf(base.FREQ(pin), 3, 1, return_str);
    else if(strcmp(commands[0],"tone") == 0)
      dtostrf(base.TONE(pin, level, duration, startstop), 3, 1, return_str);
  #else
    else if(strcmp(commands[0],"write") == 0)
      dtostrf(ERROR_COMMAND_NOT_ACTIVATED, 4, 0, return_str);
    else if(strcmp(commands[0],"read") == 0)
      dtostrf(ERROR_COMMAND_NOT_ACTIVATED, 4, 0, return_str);
    else if(strcmp(commands[0],"analog") == 0)
      dtostrf(ERROR_COMMAND_NOT_ACTIVATED, 4, 0, return_str);
    else if(strcmp(commands[0],"pwm") == 0)
      dtostrf(ERROR_COMMAND_NOT_ACTIVATED, 4, 0, return_str);
    else if(strcmp(commands[0],"freq") == 0)
      dtostrf(ERROR_COMMAND_NOT_ACTIVATED, 4, 0, return_str);
    else if(strcmp(commands[0],"tone") == 0)
      dtostrf(ERROR_COMMAND_NOT_ACTIVATED, 4, 0, return_str);
  #endif

  // GPIO EXPANDER
  // GPIO EXPANDER BASED ON PCF8575/C (or PCF8574 with some modifications)
  // Pins 0-15
  // Addresses 0-7
  #ifdef enable_expander
    else if(strcmp(commands[0],"expwrite") == 0)
      dtostrf(gpio.SET(pin, level, address), 1, 0, return_str);
    else if(strcmp(commands[0],"expread") == 0)
      dtostrf(gpio.READ(pin, address), 1, 0, return_str);
  #else
    else if(strcmp(commands[0],"expwrite") == 0)
      dtostrf(ERROR_COMMAND_NOT_ACTIVATED, 4, 0, return_str);
    else if(strcmp(commands[0],"expread") == 0)
      dtostrf(ERROR_COMMAND_NOT_ACTIVATED, 4, 0, return_str);
  #endif
  
  // TEMPERATURE/HUMIDITY
  // READ TEMPERATURE, HUMIDITY AND HEAT INDEX USING DHT22
  // commands[2] should be 0 for °C
  #ifdef enable_temp
    else if(strcmp(commands[0],"temp") == 0)
    {
      char temperature[10], humidity[10], heatindex[10];
      dtostrf(temp.READ(pin, level, TEMP), 3, 1, temperature);
      dtostrf(temp.READ(pin, level, HUM),  3, 1, humidity);
      dtostrf(temp.READ(pin, level, HEAT), 3, 1, heatindex);
      sprintf(return_str, "%s;%s;%s", temperature, humidity, heatindex);
    }
  #else
    else if(strcmp(commands[0],"temp") == 0)
      dtostrf(ERROR_COMMAND_NOT_ACTIVATED, 4, 0, return_str);
  #endif

  // BAROMETRIC PRESSURE
  // READ BAROMETRIC PRESSURE USING BMP280
  #ifdef enable_baro
    else if((strcmp(commands[0],"presbmp") == 0) || (strcmp(commands[0],"presbme") == 0))
    {
      bool sensor = strcmp(commands[0],"presbme") == 0 ? true : false;
      char pressure[10], temperature[10], altitude[10], humidity[10], heatindex[10], dewpoint[10];

      int units = atoi(commands[1]);

      dtostrf(baro.READ(sensor, units, PRES), 5, 3, pressure);    // kPa
      dtostrf(baro.READ(sensor, units, TEMP), 3, 1, temperature); // °C or °F
      dtostrf(baro.READ(sensor, units, ALT), 1, 0, altitude);    // m

      // Returns 0.0 if reading on a BMP280 sensor
      // Returns correct values for a BME280 sensor
      dtostrf(baro.READ(sensor, units, HUM), 3, 1, humidity);    // %
      dtostrf(baro.READ(sensor, units, HEAT), 3, 1, heatindex);   // °C or °F*/
      dtostrf(baro.READ(sensor, units, DEW), 3, 1, dewpoint);   // °C or °F*/

      if(sensor)
        sprintf(return_str, "%s;%s;%s;%s;%s", pressure, temperature, altitude, humidity, heatindex);
      else
        sprintf(return_str, "%s;%s;%s", pressure, temperature, altitude);
    }
  #else
    else if((strcmp(commands[0],"presbmp") == 0) || (strcmp(commands[0],"presbme") == 0))
      dtostrf(ERROR_COMMAND_NOT_ACTIVATED, 4, 0, return_str);
  #endif

  // STGC DECODER
  // READ SINGLE TRACK GRAY CODE ENCODER
  #ifdef enable_stgc
    else if(strcmp(commands[0],"stgc") == 0)
      sprintf(return_str, "%s", STGC.READ(atoi(commands[1]), atoi(commands[2]), atoi(commands[3]), atoi(commands[4]), atoi(commands[5]), atoi(commands[6])));
  #else
    else if(strcmp(commands[0],"stgc") == 0)
      dtostrf(ERROR_COMMAND_NOT_ACTIVATED, 4, 0, return_str);
  #endif

  // OLED DISPLAY WITH ASCII ONLY LIBRARIES
  // This function is predefined for 2, 5 or 8 lines:
  // OLED DISPLAY USING SSD1306 128x64 (maybe 32 later)
  #ifdef enable_oled
    else if(strcmp(commands[0],"oled") == 0)
      dtostrf(OLED.SEND(atoi(commands[1]), commands[2], commands[3], commands[4], commands[5], commands[6], commands[7], commands[8], commands[9]), 1, 0, return_str);
  #else
    else if(strcmp(commands[0],"oled") == 0)
      dtostrf(ERROR_COMMAND_NOT_ACTIVATED, 4, 0, return_str);
  #endif

  else
  {
    #ifdef DEBUG
      Serial.println("FAIL TO FIND FUNCTION");
    #endif
    dtostrf(ERROR_UNDEFINED_COMMAND, 4, 0, return_str);
  } 
}

