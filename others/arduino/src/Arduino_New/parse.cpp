#include <stdlib.h>
#include "misc.h"
#include "string.h"
#include <Arduino.h>  

#define NUMBER_OF_COMMANDS 10
#define COMMAND_LENGTH 16

char** parseData(const char* stringData)
{
  int delimiters[NUMBER_OF_COMMANDS];
  int delimiter_qty = 0;

  char** commands = (char**)malloc(NUMBER_OF_COMMANDS * sizeof(char*));
  for (int i =0 ; i < NUMBER_OF_COMMANDS; ++i)
    commands[i] = (char*)malloc(COMMAND_LENGTH * sizeof(char));
  
  // Blank all string tables
  for(int i=0; i<NUMBER_OF_COMMANDS; i++)
  {
    delimiters[i] = 0;
    commands[i][0] = NULL;
  }

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
      commands[0][delimiters[0]] = NULL;
    }
    else if(i==delimiter_qty)
      strcpy(commands[i], stringData+delimiters[i-1]+1);      //stringData.substring(delimiters[i-1]+1);
    else
    {
      nextPosition = delimiters[i-1]+1;
      len = delimiters[i]-nextPosition;
      strncpy(commands[i], stringData+nextPosition, len);    //stringData.substring(delimiters[i-1]+1, delimiters[i]);
      commands[i][len] = NULL;
    }

    
    
    //#ifdef DEBUG
      Serial.print("Command #");
      Serial.print(i+1);
      Serial.print(": ");
      Serial.println(commands[i]);
    //#endif
  }

  return commands;
}
