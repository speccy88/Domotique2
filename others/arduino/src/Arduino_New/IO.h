#include "parse.h"
  
class ioclass
{
  public:
    ioclass(char (*commands_ptr)[COMMAND_LENGTH]);
    void process(char* return_str);
  private:
    char (*commands)[COMMAND_LENGTH];
    char *result;
};

extern ioclass* io;
