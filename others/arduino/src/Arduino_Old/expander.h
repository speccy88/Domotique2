#ifndef expander_h
  #define expander_h
  
  class gpioclass
  {
    public:
      gpioclass();
      int SET(int pin, unsigned int level, int address);
      int READ(int pin, int address);
  };
  
  extern gpioclass gpio;

#endif
