#include "Arduino.h"
#include "temp.h"

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
  else
  {
    Serial.println("ERROR : Invalid Units");
    return(9994); // Invalid units
  }
  
  float hum = dht.readHumidity();
  float hi = dht.computeHeatIndex(temp, hum);

  if (isnan(temp) || isnan(hum) || isnan(hi) || temp==997)    // if values are not numbers
  {
    Serial.println("ERROR : Read failed");
    return(9995); // Reading error
  }
  else
  {
    switch(request)
    {
      case 1: // Temperature
      {
        Serial.println("Command: Read Temperature/Humidity Success");
        Serial.print("Temp: ");
        Serial.print(temp, 1);
        Serial.print(" *");
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
        Serial.print(" *");
        Serial.println(units);
        return((int)(hi*10));
      }
      break;
      default: // Error
      {
        return(9996); // Invalid command
      }
      break;
    }
  }
}

tempclass temp = tempclass();

