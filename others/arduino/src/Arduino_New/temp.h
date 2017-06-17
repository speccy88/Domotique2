#ifndef temp_h
  #define temp_h
  
  class tempclass
  {
    public:
      tempclass(char **commands_ptr);
      int TEMPERATURE(int pin, char units, int request);
      void process(char* return_str);
    private:
      char **commands;
      char *result;
  };
  
  extern tempclass temp;

#endif
