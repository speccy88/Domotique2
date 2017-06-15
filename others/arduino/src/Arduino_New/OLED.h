#ifndef OLED_h
  #define OLED_h
  
  class oledclass
  {
    public:
      oledclass();
      int SEND(int NbrLines, String Line1, String Line2, String Line3, String Line4, String Line5, String Line6, String Line7, String Line8);
  };
  
  extern oledclass OLED;

#endif
