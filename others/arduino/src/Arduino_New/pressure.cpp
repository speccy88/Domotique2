/***************************************************************************
  This is a library for the BMP280 humidity, temperature & pressure sensor

  Designed specifically to work with the Adafruit BMEP280 Breakout 
  ----> http://www.adafruit.com/products/2651

  These sensors use I2C or SPI to communicate, 2 or 4 pins are required 
  to interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Limor Fried & Kevin Townsend for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/
#include "Arduino.h"
#include "pressure.h"

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

presclass::presclass(){}

int presclass::PRES(int pin)
{
  float pres = 0;
  // For I2C, uncomment line
  //Adafruit_BMP280 bmp; // I2C

  // For SPI hardware, uncomment lines
  #define BMP_CS pin
  Adafruit_BMP280 bmp(BMP_CS); // hardware SPI  

  // For SPI on different digital pins, uncomment lines
  //#define BMP_CS pin
  //#define BMP_SCK 13
  //#define BMP_MISO 12
  //#define BMP_MOSI 11 
  //Adafruit_BMP280 bmp(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK);

  if (!bmp.begin())  
  {
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    return(9992); //
  }

  pres = bmp.readPressure();

  if(isnan(pres))
  {
    Serial.println("ERROR : Read failed");
    return(9995); // Reading error
  }  

  Serial.print(F("Pressure: "));
  Serial.print(pres);
  Serial.println(" Pa");

  return(pres/100); // Pressure is read in Pa, turns it into kPa * 10 to read one decimal
}

presclass pressure = presclass();
