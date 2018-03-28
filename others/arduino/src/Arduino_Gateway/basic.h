#ifndef BASIC_H
#define BASIC_H

#include "command_template.h"

class Basic: public CommandTemplate
{
  public:
    Basic();
    void Process();
  private:
    char *result;
    
    int Set(int pin, unsigned int level);
    int Status(int pin);
    int Read(int pin);
    int Analog(int pin);
    int Pwm(int pin, unsigned int level);
    double Freq(int pin);
    int Tone(int pin, unsigned int level, unsigned long duration, int startstop);
};

#endif
