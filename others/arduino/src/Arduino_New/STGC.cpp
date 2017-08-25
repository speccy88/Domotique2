#include "Arduino.h"
#include "STGC.h"

#include "error_codes.h"                                                               // Includes error codes and pins definitions (min/max pin numbers)
#include "ip_address.h"


// Orientation lookup table , 0 and 31st indexes are invalid and returns "-1"
int table[] = { -1,   0,   72,  12,  144, 132,   84,  120,
               216, 348,  204,  24,  156, 168,  192,  180,
               288, 300,   60,  48,  276, 312,   96,  108,
               228, 336,  240,  36,  264, 324,  252,   -1};

stgcclass::stgcclass(){}

char* stgcclass::READ(int pin1, int pin2, int pin3, int pin4, int pin5, int zero)
{
  zero = constrain(abs(zero), 0, 360);
  bool pin_ok;
  int pins[5];
  int index = 0;
  pin_ok = false;

  if((dig_lo_min <= pin1 <= dig_lo_max) || (dig_hi_min <= pin1 <= dig_hi_max))
    if((dig_lo_min <= pin2 <= dig_lo_max) || (dig_hi_min <= pin2 <= dig_hi_max))
      if((dig_lo_min <= pin3 <= dig_lo_max) || (dig_hi_min <= pin3 <= dig_hi_max))
        if((dig_lo_min <= pin4 <= dig_lo_max) || (dig_hi_min <= pin4 <= dig_hi_max))
          if((dig_lo_min <= pin5 <= dig_lo_max) || (dig_hi_min <= pin5 <= dig_hi_max))
            pin_ok = true;

  if(!pin_ok)
    return(char(ERROR_INVALID_DIGITAL_PIN));

  pinMode(pin1, INPUT_PULLUP);
  pinMode(pin2, INPUT_PULLUP);
  pinMode(pin3, INPUT_PULLUP);
  pinMode(pin4, INPUT_PULLUP);
  pinMode(pin5, INPUT_PULLUP);
  
  pins[0] = (int)digitalRead(pin1);
  pins[1] = (int)digitalRead(pin2);
  pins[2] = (int)digitalRead(pin3);
  pins[3] = (int)digitalRead(pin4);
  pins[4] = (int)digitalRead(pin5);
    
  for(int i = 0; i < 5; i++)
    if(pins[i] == 1)
      index |= (1 << i);

  // displays orientation using table lookup
  #ifdef DEBUG
    Serial.println(table[index]);
  #endif

  int angle_read = table[index];

  if(angle_read == -1)
    return(char(ERROR_SENSOR_READ));

  angle_read -= zero;
  if(angle_read < 0)
    angle_read += 360;

  if(angle_read > 360 || angle_read < 0)
    return(char(ERROR_SENSOR_READ));

  return(COMPASS(angle_read));
}

char* stgcclass::COMPASS(int angle)
{
  char* compass_str;
  #ifdef campass_enable
    if(angle > 337.5 || angle <= 22.5)  //N
      compass_str = "N";
    if(angle > 22.5 && angle <= 67.5)   //NE
      compass_str = "NE";
    if(angle > 67.5 && angle <= 112.5)  //E
      compass_str = "E";
    if(angle > 112.5 && angle <= 157.5) //SE
      compass_str = "SE";
    if(angle > 157.5 && angle <= 202.5) //S
      compass_str = "S";
    if(angle > 202.5 && angle <= 247.5) //SW
      compass_str = "SW";
    if(angle > 247.5 && angle <= 292.5) //W
      compass_str = "W";
    if(angle > 292.5 && angle <= 337.5) //NW
      compass_str = "NW";
  #else
    //Todo
  #endif

  return(compass_str);
}

stgcclass STGC = stgcclass();
