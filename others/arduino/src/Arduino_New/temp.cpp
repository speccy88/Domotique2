#include "Arduino.h"
#include "temp.h"

#include "error_codes.h"                                                               // Includes error codes and pins definitions (min/max pin numbers)

#include "DHT.h"
#define DHTTYPE DHT22                                                                  // DHT 22  (AM2302), AM2321

tempclass::tempclass(){}

int tempclass::TEMPERATURE(int pin, String units, int request)
{
  DHT dht(pin, DHTTYPE);
  dht.begin();

  float temp = 0;
  
  if(units == "C")
    temp = dht.readTemperature(false);
  else if(units == "F")
    temp = dht.readTemperature(true);
  else if(units == "K")
    temp = dht.readTemperature(false) + 273.15;
  else
    return(ERROR_UNDEFINED_COMMAND); // Invalid units

  float hum = dht.readHumidity();
  float hi = dht.computeHeatIndex(temp, hum);

  if (isnan(temp) || isnan(hum) || isnan(hi))    // if values are not numbers
    return(ERROR_SENSOR_READ); // Reading error
  else
  {
    switch(request)
    {
      case 1: // Temperature
      {
        Serial.println("Command: Read Temperature/Humidity Success");
        Serial.print("Temp: ");
        Serial.print(temp, 1);
        Serial.print(" ");
        Serial.println(units);
        return((int)(temp*10));
      }
      break;
      case 2: // Humidity
      {
        Serial.print("Humidity: ");
        Serial.print(hum, 1);
        Serial.println(" %");
        return((int)(hum*10));
      }
      break;
      case 3: // Heat Index
      {
        Serial.print("Humidex: ");
        Serial.print(hi, 1);
        Serial.print(" ");
        Serial.println(units);
        return((int)(hi*10));
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

