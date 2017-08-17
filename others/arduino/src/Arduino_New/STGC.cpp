#include "Arduino.h"
#include "STGC.h"

#include "error_codes.h"                                                               // Includes error codes and pins definitions (min/max pin numbers)

bool pin_ok;

int pins[5];
int index;

// Orientation lookup table , 0 and 31st indexes are invalid and returns "-1"
int table[] = { -1,   0,   72,  12,  144, 132,   84,  120,
               216, 348,  204,  24,  156, 168,  192,  180,
               288, 300,   60,  48,  276, 312,   96,  108,
               228, 336,  240,  36,  264, 324,  252,   -1};

stgcclass::stgcclass(){}

int stgcclass::READ(int pin1, int pin2, int pin3, int pin4, int pin5, int zero)
{
  index = 0;
  pin_ok = false;

  if((dig_lo_min <= pin1 <= dig_lo_max) || (dig_hi_min <= pin1 <= dig_hi_max))
    if((dig_lo_min <= pin2 <= dig_lo_max) || (dig_hi_min <= pin2 <= dig_hi_max))
      if((dig_lo_min <= pin3 <= dig_lo_max) || (dig_hi_min <= pin3 <= dig_hi_max))
        if((dig_lo_min <= pin4 <= dig_lo_max) || (dig_hi_min <= pin4 <= dig_hi_max))
          if((dig_lo_min <= pin5 <= dig_lo_max) || (dig_hi_min <= pin5 <= dig_hi_max))
            pin_ok = true;

  if(!pin_ok)
    return(ERROR_INVALID_DIGITAL_PIN);

  pinMode(pin1, INPUT);
  pinMode(pin2, INPUT);
  pinMode(pin3, INPUT);
  pinMode(pin4, INPUT);
  pinMode(pin5, INPUT);
  
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
  
  return(table[index]);
}

stgcclass STGC = stgcclass();
