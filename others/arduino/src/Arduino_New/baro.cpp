#include "Arduino.h"
#include "baro.h"

#include "error_codes.h"                                                               // Includes error codes and pins definitions (min/max pin numbers)

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

baroclass::baroclass(){}

double baroclass::PRES(int pin)
{
  int BMP_CS = pin;
  float pres = 0;
  
  // For I2C, uncomment line
  //Adafruit_BMP280 bmp; // I2C

  // For SPI hardware, uncomment lines
  Adafruit_BMP280 bmp(BMP_CS); // hardware SPI  

  // For SPI on different digital pins, uncomment lines
  //int BMP_SCK = 13
  //int BMP_MISO = 12
  //int BMP_MOSI = 11 
  //Adafruit_BMP280 bmp(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK);

  if (!bmp.begin())  
  {
    #ifdef DEBUG
      Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    #endif
  
    return(ERROR_SENSOR_READ); //Invalid sensor or not found
  }

  pres = bmp.readPressure();

  if(isnan(pres))
  {
    #ifdef DEBUG
      Serial.println("ERROR : Read failed");
    #endif
    
    return(ERROR_SENSOR_READ); // Reading error
  }  

  #ifdef DEBUG
    Serial.print(F("Pressure: "));
    Serial.print(pres);
    Serial.println(" Pa");
  #endif
 
  return(pres/1000); // Pressure is read in Pa, turns it into kPa and accept 3 decimals in the double to string conversion
}

baroclass baro = baroclass();
