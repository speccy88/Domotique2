#ifndef digital_h
  #define digital_h
  
  
  class digclass
  {
    public:
      digclass();
      int SET(int pin, bool level);
      int READ(int pin);
      int STATUS(int pin);
  };
  
  extern digclass digital;

#endif
