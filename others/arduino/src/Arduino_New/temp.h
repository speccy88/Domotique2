//#include "parse.h"
  
class tempclass
{
  public:
    //tempclass(char (*commands_ptr)[COMMAND_LENGTH]);
    tempclass();
    int TEMPERATURE(int pin, char units, int request);
    //void process(char* return_str);
  /*private:
    char (*commands)[COMMAND_LENGTH];
    char *result;*/
};

extern tempclass/***/ temp;


