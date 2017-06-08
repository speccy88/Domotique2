#ifndef OLED128x64_h
  #define OLED128x64_h
  
  class oledclass
  {
    public:
      oledclass();
      int SEND64(String Line1, String Line2, String Line3, String Line4, String Line5);
  };
  
  extern oledclass OLED;

#endif
