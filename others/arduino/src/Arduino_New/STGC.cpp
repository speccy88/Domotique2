#include "Arduino.h"
#include "STGC.h"

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

/*
  pins[0] = one;
  pins[1] = two;
  pins[2] = three;
  pins[3] = four;
  pins[4] = five;
*/  
  for(int i = 0; i < 5; i++)
    if(pins[i] == 1)
      index |= (1 << i);
  // displays orientation using table lookup
  Serial.println(table[index]);
  
  return(table[index]);
}

stgcclass STGC = stgcclass();
