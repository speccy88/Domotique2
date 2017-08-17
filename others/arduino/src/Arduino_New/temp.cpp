#include "Arduino.h"
#include "temp.h"

#include "error_codes.h"                                                               // Includes error codes and pins definitions (min/max pin numbers)

#include "DHT.h"
#define DHTTYPE DHT22                                                                  // DHT 22  (AM2302), AM2321

tempclass::tempclass(){}

double tempclass::READ(int pin, int units, int request)
{
  #ifdef DEBUG
    Serial.println("INSIDE TEMP");
    Serial.println(pin);
    Serial.println(units);
  #endif
  
  DHT dht(pin, DHTTYPE);
  dht.begin();

  double temp = -1000.0;
  
  if(units == 0)      //°C
    temp = dht.readTemperature(false);
  else if(units == 1) //°F
    temp = dht.readTemperature(true);
  else
    return(ERROR_UNDEFINED_COMMAND); // Invalid units

  #ifdef DEBUG
    Serial.println(temp);
  #endif

  double hum = dht.readHumidity();
  double hi = dht.computeHeatIndex(temp, hum);

  if (isnan(temp) || isnan(hum) || isnan(hi))    // if values are not numbers
    return(ERROR_SENSOR_READ); // Reading error
  else
  {
    #ifdef DEBUG
      Serial.println("INSIDE CASE");
      Serial.println(request);
    #endif
    
    switch(request)
    {
      case TEMP: // Temperature
      {
        #ifdef DEBUG
          Serial.println("Command: Read Temperature/Humidity Success");
          Serial.print("Temp: ");
          Serial.print(temp, 1);
          Serial.print(" ");
          Serial.println(units);
        #endif
        
        return(temp);
      }
      break;
      case HUM: // Humidity
      {
        #ifdef DEBUG
          Serial.print("Humidity: ");
          Serial.print(hum, 1);
          Serial.println(" %");
        #endif
        
        return(hum);
      }
      break;
      case HEAT: // Heat Index
      {
        #ifdef DEBUG
          Serial.print("Humidex: ");
          Serial.print(hi, 1);
          Serial.print(" ");
          Serial.println(units);
        #endif

        return(hi);
      }
      break;
      default: // Error
      {
        return(ERROR_SENSOR_REQUEST_FAIL); // Invalid command
      }
      break;
    }
  }
}

tempclass temp = tempclass();

