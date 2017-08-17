class baseclass
{
  public:
    baseclass();
    int SET(int pin, unsigned int level);
    int READ(int pin);
    int ANALOG(int pin);
    int PWM(int pin, unsigned int level);
    double FREQ(int pin);
    int TONE(int pin, unsigned int level, unsigned long duration, int startstop);
};

extern baseclass base;
