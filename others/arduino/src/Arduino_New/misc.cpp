//misc.cpp
#include <stdlib.h>
#include <stdio.h>
#include "arduino.h"

#include "string.h"
#include "math.h"

int getIndex(const char* string, char toFind)
{
  const char *ptr = strchr(string, toFind);

  if (ptr == NULL)
    return -1;
  else
    return (ptr - string);
}

char* floatToString(float f, int n)
{
  int integer_part; 
  int fractional_part;

  integer_part = floor(f);
  fractional_part = (f-(integer_part))*pow(10,n);

  char* str_buffer = (char*)malloc(16 * sizeof(char));
  sprintf(str_buffer, "%d.%d", integer_part, fractional_part);
  return str_buffer;
}

