// command_template.h
#ifndef COMMAND_TEMPLATE_H
#define COMMAND_TEMPLATE_H

class CommandTemplate
{
  int test;

protected:
  char* name;

public:
  CommandTemplate();
  virtual void Process();
};

#endif

