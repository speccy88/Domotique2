#include "arduino.h"

double convertCtoF(double c)
{
  return c * 1.8 + 32;
}

double convertFtoC(double f)
{
  return (f - 32) * 0.55555;
}

//Temperature should be sent in F
double calcHeatIndex(double temperature, double humidity)
{
  double hi;
  hi = 0.5 * (temperature + 61.0 + ((temperature - 68.0) * 1.2) + (humidity * 0.094));

  if(hi > 79)
  {
    hi = -42.379 +
             2.04901523 * temperature +
            10.14333127 * humidity +
            -0.22475541 * temperature*humidity +
            -0.00683783 * pow(temperature, 2) +
            -0.05481717 * pow(humidity, 2) +
             0.00122874 * pow(temperature, 2) * humidity +
             0.00085282 * temperature * pow(humidity, 2) +
            -0.00000199 * pow(temperature, 2) * pow(humidity, 2);

    if((humidity < 13) && (temperature >= 80.0) && (temperature <= 112.0))
      hi -= ((13.0 - humidity) * 0.25) * sqrt((17.0 - abs(temperature - 95.0)) * 0.05882);

    else if((humidity > 85.0) && (temperature >= 80.0) && (temperature <= 87.0))
      hi += ((humidity - 85.0) * 0.1) * ((87.0 - temperature) * 0.2);
  }
  return(hi);
}

//Temperature should be sent in C
double calcDewPoint(double temperature, double humidity)
{
  double B;
  double dp;

  
  B = 17.27 * temperature;
  B /= 237.3 + temperature;
  B += log(0.01 * humidity);
  B /= 17.27;

  dp = (237.3 * B) / (1 - B);

  return(dp);
}
