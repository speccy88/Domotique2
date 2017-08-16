#include "Arduino.h"
#include "temp.h"

#include "error_codes.h"                                                               // Includes error codes and pins definitions (min/max pin numbers)

#include "DHT.h"
#define DHTTYPE DHT22                                                                  // DHT 22  (AM2302), AM2321

//tempclass* temp;

/*tempclass::tempclass(char (*commands_ptr)[COMMAND_LENGTH])
{
  commands = commands_ptr;
}

void tempclass::process(char* return_str)
{
  Serial.println("PROCESS CLASS");
  int pin = atoi(commands[1]);
  char units = (char)*commands[2];
  
  sprintf(return_str, "%d", TEMPERATURE(pin, units, 1));
  //sprintf(return_str, "%d,%d,%d", TEMPERATURE(pin, units, 1), TEMPERATURE(pin, units, 2), TEMPERATURE(pin, units, 3));
}*/

tempclass::tempclass(){}

int tempclass::TEMPERATURE(int pin, char units, int request)
{
  #ifdef DEBUG
    Serial.println("INSIDE TEMP");
    Serial.println(pin);
    Serial.println(units);
  #endif
  
  DHT dht(pin, DHTTYPE);
  dht.begin();

  float temp = -1000;
  
  if(units == 'C')
    temp = dht.readTemperature(false);
  else if(units == 'F')
    temp = dht.readTemperature(true);
  else
    return(ERROR_UNDEFINED_COMMAND); // Invalid units

  #ifdef DEBUG
    Serial.println(temp);
  #endif

  float hum = dht.readHumidity();
  float hi = dht.computeHeatIndex(temp, hum);

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
      case 1: // Temperature
      {
        #ifdef DEBUG
          Serial.println("Command: Read Temperature/Humidity Success");
          Serial.print("Temp: ");
          Serial.print(temp, 1);
          Serial.print(" ");
          Serial.println(units);
        #endif
        
        return((int)(temp*10));
      }
      break;
      case 2: // Humidity
      {
        #ifdef DEBUG
          Serial.print("Humidity: ");
          Serial.print(hum, 1);
          Serial.println(" %");
        #endif
        
        return((int)(hum*10));
      }
      break;
      case 3: // Heat Index
      {
        #ifdef DEBUG
          Serial.print("Humidex: ");
          Serial.print(hi, 1);
          Serial.print(" ");
          Serial.println(units);
        #endif

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

