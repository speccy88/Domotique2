static byte myip[] = { 192,168,0,100 };                                                // Static IP Address (Set this to whatever IP you want
static byte gwip[] = { 192,168,0,1 };                                                  // Static Gateway IP Address
static byte subnet[] = { 255,255,255,0 };                                              // Static Subnet Mask
static byte mymac[] = { 0xFF,0xFF,0xFF,0xFF,0xFF,0x01 };                               // Static MAC Address - MUST NOT have 2 identical MAC address on same network

//Disable unused functions to free up some memory on your Arduino project
  #define enable_base         //8% memory use --> digital in/out, analog, pwm, frequency count, tone
  #define enable_temp         //15% memory use --> Works for DHT22 ONLY
  #define enable_baro         //19% memory use --> Works for I2C address 0x76, code modifications needed for SPI (see github library) - Select BME or BMP in function
  #define enable_expander     //2% memory use  --> Works for PCF8575C ONLY, code modifications needed for PCF8575 (see github library)
  #define enable_stgc         //6% memory use  --> UNTESTED
  #define enable_oled         //8% memory use  --> UNTESTED - Works for SSD1306 ONLY using text library only

  #ifdef enable_baro
    #define baro_pres
    #define baro_temp
    #define baro_alt
  #endif
