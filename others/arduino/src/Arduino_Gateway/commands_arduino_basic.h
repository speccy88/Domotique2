class Commands_Arduino_Basic
{
  public:
    Commands_Arduino_Basic();
    void Process(char* return_str);
  private:
    char *result;
    
    int Set(int pin, unsigned int level);
    int Read(int pin);
    int Analog(int pin);
    int Pwm(int pin, unsigned int level);
    double Freq(int pin);
    int Tone(int pin, unsigned int level, unsigned long duration, int startstop);
};
