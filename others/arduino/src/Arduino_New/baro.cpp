//PINOUT:
//SDA = A4
//SCL = A5
#include "Arduino.h"
#include "baro.h"
#include "ip_address.h"

#include "error_codes.h"                                                               // Includes error codes and pins definitions (min/max pin numbers)

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>

// BME280 (PRES, TEMP, ALTITUDE)
#include <Adafruit_BMP280.h>
Adafruit_BMP280 barosensor; // I2C

// BME280 (PRES, TEMP, HUMIDITY, ALTITUDE)
//#include <Adafruit_BME280.h>
//Adafruit_BME280 barosensor; // I2C

baroclass::baroclass(){}

double baroclass::READ(int select)
{
  Serial.println(select);
  if(select < 1 || select > 4)
    return(ERROR_SENSOR_REQUEST_FAIL);

  if(!barosensor.begin())  
  {
    #ifdef DEBUG
      Serial.println("Could not find a valid BMx280 sensor, check wiring ans sensor!");
    #endif
  
    return(ERROR_SENSOR_READ); //Invalid sensor or not found
  }

  if(select == 1)
    return(baro.PRESSURE());
  if(select == 2)
    return(baro.TEMPERATURE(false));
  if(select == 3)
    return(baro.TEMPERATURE(true));
  if(select == 4)
    return(baro.ALTITUDE());
}

double baroclass::PRESSURE()
{
  #ifdef baro_pres
    float pres = barosensor.readPressure();
  
    if(isnan(pres))
      return(ERROR_SENSOR_READ); // Reading error
  
    #ifdef DEBUG
      Serial.print(F("Pressure: "));
      Serial.print(pres);
      Serial.println(" Pa");
    #endif
   
    return(pres/1000); // Pressure is read in Pa, turns it into kPa and accept 3 decimals in the double to string conversion
  #else
    return(ERROR_COMMAND_NOT_ACTIVATED);
  #endif
}

double baroclass::TEMPERATURE(bool units)
{
  #ifdef baro_temp
    float temp = barosensor.readTemperature();
  
    if(isnan(temp))
      return(ERROR_SENSOR_READ); // Reading error
  
    if(units)
      temp = ((9.0/5.0) * temp) + 32.0;
  
    #ifdef DEBUG
      Serial.print(F("Temperature: "));
      Serial.print(temp);
      if(units)
        Serial.println(" *F");
      else
        Serial.println(" *C");
    #endif
   
    return(temp); // Temperature is read in °C
  #else
    return(ERROR_COMMAND_NOT_ACTIVATED);
  #endif
}

double baroclass::ALTITUDE()
{
  #ifdef baro_alt
    float alt = barosensor.readAltitude(CALIBRATION);
  
    if(isnan(alt))
      return(ERROR_SENSOR_READ); // Reading error
  
    #ifdef DEBUG
      Serial.print(F("Altitude: "));
      Serial.print(alt);
      Serial.println(" m");
    #endif
   
    return(alt); // Pressure is read in Pa, turns it into kPa and accept 3 decimals in the double to string conversion
  #else
    return(ERROR_COMMAND_NOT_ACTIVATED);
  #endif
}

baroclass baro = baroclass();
