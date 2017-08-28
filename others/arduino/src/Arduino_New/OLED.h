#ifndef OLED_h
  #define OLED_h
  
  class oledclass
  {
    public:
      oledclass();
      int SEND(int NbrLines, char* Line1, char* Line2, char* Line3, char* Line4, char* Line5, char* Line6, char* Line7, char* Line8);
  };
  
  extern oledclass OLED;

#endif
