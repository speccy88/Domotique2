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

int stgcclass::READ(int one, int two, int three, int four, int five)
{
  index = 0;

  if((dig_lo_min <= one <= dig_lo_max) || (dig_hi_min <= one <= dig_hi_max))
    if((dig_lo_min <= two <= dig_lo_max) || (dig_hi_min <= two <= dig_hi_max))
      if((dig_lo_min <= three <= dig_lo_max) || (dig_hi_min <= three <= dig_hi_max))
        if((dig_lo_min <= four <= dig_lo_max) || (dig_hi_min <= four <= dig_hi_max))
          if((dig_lo_min <= five <= dig_lo_max) || (dig_hi_min <= five <= dig_hi_max))
            pin_ok = true;

  if(pin_ok)
  {
    pinMode(one, INPUT);
    pinMode(two, INPUT);
    pinMode(three, INPUT);
    pinMode(four, INPUT);
    pinMode(five, INPUT);
    
    pins[0] = (int)digitalRead(one);
    pins[1] = (int)digitalRead(two);
    pins[2] = (int)digitalRead(three);
    pins[3] = (int)digitalRead(four);
    pins[4] = (int)digitalRead(five);
  }
  else
    return(ERROR_INVALID_DIGITAL_PIN);
    
  for(int i = 0; i < 5; i++)
    if(pins[i] == 1)
      index |= (1 << i);

  // displays orientation using table lookup
  Serial.println(table[index]);
  
  return(table[index]);
}

stgcclass STGC = stgcclass();
