class baroclass
{
  public:
    baroclass();
    double READ(int select);
    double PRESSURE();
    double TEMPERATURE(bool units);
    double ALTITUDE();
};

extern baroclass baro;
