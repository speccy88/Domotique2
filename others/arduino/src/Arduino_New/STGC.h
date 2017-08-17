#ifndef STGC_h
  #define STGC_h
  
  class stgcclass
  {
    public:
      stgcclass();
      int READ(int pin1, int pin2, int pin3, int pin4, int pin5, int zero);
  };
  
  extern stgcclass STGC;

#endif
