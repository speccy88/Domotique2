#ifndef STGC_h
  #define STGC_h
  
  class stgcclass
  {
    public:
      stgcclass();
      char* READ(int pin1, int pin2, int pin3, int pin4, int pin5, int zero);
      char* COMPASS(int angle);
  };
  
  extern stgcclass STGC;

#endif
