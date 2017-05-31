#ifndef digital_h
  #define digital_h
  
  
  class digclass
  {
    public:
      digclass();
      int SET(int pin, int level);
      int READ(int pin);
      int STATUS(int pin);
      int ANALOG(int pin);
      int PWM(int pin, int level);
      int FREQ(int pin);
      int TONE(int pin, int level, int duration, String startstop);
  };
  
  extern digclass digital;

#endif
