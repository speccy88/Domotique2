#ifndef temp_h
  #define temp_h
  
  
  class tempclass
  {
    public:
      tempclass();
      int TEMPERATURE(int pin, String units, int request);
  };
  
  extern tempclass temp;

#endif
