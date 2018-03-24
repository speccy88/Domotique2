class baroclass
{
  public:
    baroclass();
    double READ(bool sensor, int units, int select);
    double PRESSURE(bool sensor);
    double TEMPERATURE(bool sensor, bool isFahrenheit);
    double ALTITUDE(bool sensor);
    double HUMIDITY(bool sensor);
    double HEATINDEX(bool sensor, bool isFahrenheit);
    double DEWPOINT(bool sensor, bool isFahrenheit);
};

extern baroclass baro;
