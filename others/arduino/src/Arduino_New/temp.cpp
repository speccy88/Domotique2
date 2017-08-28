#include "Arduino.h"
#include "temp.h"
#include "meteo.h"

#include "error_codes.h"                                                               // Includes error codes and pins definitions (min/max pin numbers)

#include "DHT.h"
#define DHTTYPE DHT22                                                                  // DHT 22  (AM2302), AM2321

tempclass::tempclass(){}

double tempclass::READ(int pin, int units, int select)
{
  DHT dht(pin, DHTTYPE);
  dht.begin();

  double temp = -1000.0;
  double hum = dht.readHumidity();
  
  if(units == 0)      //°C
    temp = dht.readTemperature(false);
  else if(units == 1) //°F
    temp = dht.readTemperature(true);
  else
    return(ERROR_UNDEFINED_COMMAND); // Invalid units

  double reply;

  if (isnan(temp) || isnan(hum))    // if values are not numbers
    return(ERROR_SENSOR); // Reading error
  else
  {
    switch(select)
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
        
        reply = temp;
        break;
      }
      case HUM: // Humidity
      {
        #ifdef DEBUG
          Serial.print("Humidity: ");
          Serial.print(hum, 1);
          Serial.println(" %");
        #endif
        
        reply = hum;
        break;
      }
      case HEAT: // Heat Index
      {
        double hi = calcHeatIndex(temp, hum); // Need to add units conversion
        #ifdef DEBUG
          Serial.print("Humidex: ");
          Serial.print(hi, 1);
          Serial.print(" ");
          Serial.println(units);
        #endif

        reply = hi;
        break;
      }
      case DEW: // Heat Index
      {
        double dp = calcDewPoint(temp, hum);  // Need to add units conversion
        #ifdef DEBUG
          Serial.print("Dewpoint: ");
          Serial.print(dp, 1);
          Serial.print(" ");
          Serial.println(units);
        #endif
        
        reply = dp;
        break;
      }
      default: // Error
      {
        reply = ERROR_SENSOR_REQUEST_FAIL; // Invalid command
        break;
      }

      return(reply);
    }
  }
}

tempclass temp = tempclass();

