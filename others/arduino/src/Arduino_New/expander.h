#ifndef expander_h
  #define expander_h
  
  class gpioclass
  {
    public:
      gpioclass();
      int SET(int pin, int level, int address);
      int READ(int pin, int address);
      int STATUS(int pin, int address);
  };
  
  extern gpioclass expander;

#endif
