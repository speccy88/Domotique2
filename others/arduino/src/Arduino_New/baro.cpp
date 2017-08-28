//PINOUT:
//SDA = A4
//SCL = A5
#include "Arduino.h"
#include "baro.h"
#include "misc.h"

#include "error_codes.h"                                                               // Includes error codes and pins definitions (min/max pin numbers)
#include "ip_address.h"

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>

#ifdef baro_bmp
  #include <Adafruit_BMP280.h>
  Adafruit_BMP280 baroBMP; // I2C BMP
#endif
#ifdef baro_bme
  #include <Adafruit_BME280.h>
  Adafruit_BME280 baroBME; // I2C BME
#endif

baroclass::baroclass(){}

// BMP280 (PRES, TEMP, ALTITUDE)
// BME280 (PRES, TEMP, ALTITUDE, HUMIDITY, HEAT INDEX)
// sensor true = BME, false = BMP
double baroclass::READ(bool sensor, int units, int select)
{ 
  //boolean isFahrenheit: true == Fahrenheit; false == Celcius
  bool isFahrenheit = units == 1 ? true : false;
  
  if(select < 1 || select > 5)
    return(ERROR_SENSOR_REQUEST_FAIL);

  if(sensor)
  {
    #ifdef baro_bme
      if(!baroBME.begin())  
        return(ERROR_SENSOR); //Invalid sensor or not found
    #else
      return(ERROR_COMMAND_NOT_ACTIVATED);
    #endif
  }
  else
  {
    #ifdef baro_bmp
      if(!baroBMP.begin())  
        return(ERROR_SENSOR); //Invalid sensor or not found
    #else
      return(ERROR_COMMAND_NOT_ACTIVATED);
    #endif
  }

  if(select == 1)
    return(baro.PRESSURE(sensor));
  if(select == 2)
    return(baro.TEMPERATURE(sensor, isFahrenheit));
  if(select == 3)
    return(baro.ALTITUDE(sensor));
  if(select == 4)
    return(sensor ? baro.HUMIDITY(sensor) : 0.0);
  if(select == 5)
    return(sensor ? baro.HEATINDEX(sensor, isFahrenheit) : 0.0);
}

// sensor true = BME, false = BMP
double baroclass::PRESSURE(bool sensor)
{
  #ifdef baro_pres
    double pres;

    #ifdef baro_bmp
      pres = sensor ? 0.0 : baroBMP.readPressure();
    #else if baro_bme
      pres = sensor ? baroBME.readPressure() : 0.0;
    #endif
  
    if(isnan(pres))
      return(ERROR_SENSOR); // Reading error
  
    #ifdef DEBUG
      Serial.print(F("Pressure: "));
      Serial.print(pres/1000);
      Serial.println(" kPa");
    #endif
   
    return(pres/1000); // Pressure is read in Pa, turns it into kPa and accept 3 decimals in the double to string conversion
  #else
    return(ERROR_COMMAND_NOT_ACTIVATED);
  #endif
}

// sensor true = BME, false = BMP
double baroclass::TEMPERATURE(bool sensor, bool isFahrenheit)
{
  #ifdef baro_temp
    double temp;

    #ifdef baro_bmp
      temp = sensor ? 0.0 : baroBMP.readTemperature();
    #endif
    #ifdef baro_bme
      temp = sensor ? baroBME.readTemperature() : 0.0;
    #endif
  
    if(isnan(temp))
      return(ERROR_SENSOR); // Reading error
  
    if(isFahrenheit)
      temp = convertCtoF(temp);
  
    #ifdef DEBUG
      Serial.print(F("Temperature: "));
      Serial.print(temp);
      Serial.println(isFahrenheit ? " *F" : " *C");
    #endif
   
    return(temp); // Temperature is read in °C
  #else
    return(ERROR_COMMAND_NOT_ACTIVATED);
  #endif
}

double baroclass::ALTITUDE(bool sensor)
{
  #ifdef baro_alt
    double alt;

    #ifdef baro_bmp
      alt = sensor ? 0.0 : baroBMP.readAltitude(CALIBRATION);
    #endif
    #ifdef baro_bme
      alt = sensor ? baroBME.readAltitude(CALIBRATION) : 0.0;
    #endif
  
    if(isnan(alt))
      return(ERROR_SENSOR); // Reading error
  
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

double baroclass::HUMIDITY(bool sensor)
{
  #ifdef baro_hum
    if(!sensor)
      return(0.0);
    
    double hum = baroBME.readHumidity();
  #else
    return(ERROR_COMMAND_NOT_ACTIVATED);
  #endif
}

//boolean isFahrenheit: True == Fahrenheit; False == Celcius
// Using both Rothfusz and Steadman's equations
// http://www.wpc.ncep.noaa.gov/html/heatindex_equation.shtml
double baroclass::HEATINDEX(bool sensor, bool isFahrenheit)
{
  #ifdef baro_hi
    if(!sensor)
      return(0.0);
      
    double hi;
    double temperature = baro.TEMPERATURE(sensor, isFahrenheit);
    double humidity = baro.HUMIDITY(sensor);
  
    if (!isFahrenheit)
      temperature = convertCtoF(temperature);
  
    hi = 0.5 * (temperature + 61.0 + ((temperature - 68.0) * 1.2) + (humidity * 0.094));
  
    if (hi > 79) {
      hi = -42.379 +
               2.04901523 * temperature +
              10.14333127 * humidity +
              -0.22475541 * temperature*humidity +
              -0.00683783 * pow(temperature, 2) +
              -0.05481717 * pow(humidity, 2) +
               0.00122874 * pow(temperature, 2) * humidity +
               0.00085282 * temperature*pow(humidity, 2) +
              -0.00000199 * pow(temperature, 2) * pow(humidity, 2);
  
      if((humidity < 13) && (temperature >= 80.0) && (temperature <= 112.0))
        hi -= ((13.0 - humidity) * 0.25) * sqrt((17.0 - abs(temperature - 95.0)) * 0.05882);
  
      else if((humidity > 85.0) && (temperature >= 80.0) && (temperature <= 87.0))
        hi += ((humidity - 85.0) * 0.1) * ((87.0 - temperature) * 0.2);
    }
  
    return isFahrenheit ? hi : convertFtoC(hi);
  #else
    return(ERROR_COMMAND_NOT_ACTIVATED);
  #endif
}

baroclass baro = baroclass();
