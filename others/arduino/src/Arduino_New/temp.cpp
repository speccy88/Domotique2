#include "Arduino.h"
#include "temp.h"

#include "error_codes.h"                                                               // Includes error codes and pins definitions (min/max pin numbers)

#include "DHT.h"
#define DHTTYPE DHT22                                                                  // DHT 22  (AM2302), AM2321

tempclass::tempclass(char **commands_ptr)
{
  commands = commands_ptr;
}

void tempclass::process(char* return_str)
{
  Serial.println("PROCESS CLASS");
  Serial.println(commands[0]);
  Serial.println(commands[1]);
  Serial.println(commands[2]);
  Serial.println(commands[3]);
  Serial.println(atoi(commands[1]));
  int pin = atoi(commands[1]);
  char units = commands[2];
  TEMPERATURE(pin, units, 1);
  
  //sprintf(return_str, "%d,%d,%d", reply[0], reply[1], reply[2]);
}


int tempclass::TEMPERATURE(int pin, char units, int request)
{
  Serial.println("INSIDE TEMP");
  Serial.println(pin);
  DHT dht(pin, DHTTYPE);
  dht.begin();

  float temp = 0;

  
  if(strcmp (units,"C") == 0)
    temp = dht.readTemperature(false);
  else if(strcmp (units,"F") == 0)
    temp = dht.readTemperature(true);
  else
    return(ERROR_UNDEFINED_COMMAND); // Invalid units

  Serial.println(temp);

  float hum = dht.readHumidity();
  float hi = dht.computeHeatIndex(temp, hum);

  if (isnan(temp) || isnan(hum) || isnan(hi))    // if values are not numbers
    return(ERROR_SENSOR_READ); // Reading error
  else
  {
    Serial.println("INSIDE CASE");
    Serial.println(request);
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



