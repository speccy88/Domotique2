#include "parse.h"
  
class digclass
{
  public:
    digclass(char (*commands_ptr)[COMMAND_LENGTH]);
    int SET(int pin, int level);
    int READ(int pin);
    int STATUS(int pin);
    int ANALOG(int pin);
    int PWM(int pin, int level);
    double FREQ(int pin);
    int TONE(int pin, int level, int duration, int startstop);
    void process(char* return_str);
  private:
    char (*commands)[COMMAND_LENGTH];
    char *result;
};

extern digclass* digital;


