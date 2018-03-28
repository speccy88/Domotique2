#ifndef GLOBALS_H
#define GLOBALS_H

#include "defines.h"
#include "basic.h"

extern char (*command_ptr)[COMMAND_LENGTH];
extern char UDP_Reply_Buffer[UDP_REPLY_BUFLEN];
extern Basic arduinoBasic;
extern int command_index;
extern CommandTemplate* Command_List[];

#endif
