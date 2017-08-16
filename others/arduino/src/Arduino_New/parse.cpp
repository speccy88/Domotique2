#include <stdlib.h>
#include "misc.h"
#include "parse.h"
#include "string.h"
#include <Arduino.h>

#include "error_codes.h"

char commands[NUMBER_OF_COMMANDS][COMMAND_LENGTH];

char (*parseData(const char* stringData))[COMMAND_LENGTH]
{
  int delimiters[NUMBER_OF_COMMANDS];
  int delimiter_qty = 0;
  
  // Blank all string tables
  for(int i=0; i<NUMBER_OF_COMMANDS; i++)
    delimiters[i] = 0;

  // Find all delimiters positions in received string
  delimiter_qty = 0;
  int lastIndex;
  int newIndex;
  for(int i=0; i<NUMBER_OF_COMMANDS; i++)
  {
    if(i==0)
      delimiters[0] = getIndex(stringData, ':');
    else 
    {
      lastIndex = delimiters[i-1]+1;
      newIndex = getIndex(stringData+lastIndex, ':');
      
      if (newIndex == -1)
        break;
      else
        delimiters[i] = lastIndex + newIndex;
    }
    
    delimiter_qty += 1;
  }

  // Split and store data according to delimiters positions
  int nextPosition;
  int len;
  for(int i=0; i<=delimiter_qty; i++)
  {
    if(i==0)
    {
      strncpy(commands[0], stringData, delimiters[0]);         //stringData.substring(0, delimiters[i]);
      commands[0][delimiters[0]] = 0;
    }
    else if(i==delimiter_qty)
      strcpy(commands[i], stringData+delimiters[i-1]+1);      //stringData.substring(delimiters[i-1]+1);
    else
    {
      nextPosition = delimiters[i-1]+1;
      len = delimiters[i]-nextPosition;
      strncpy(commands[i], stringData+nextPosition, len);    //stringData.substring(delimiters[i-1]+1, delimiters[i]);
      commands[i][len] = 0;
    }

  #ifdef DEBUG
    Serial.print("Command #");
    Serial.print(i+1);
    Serial.print(": ");
    Serial.println(commands[i]);
  #endif    
  }
  return commands;
}
