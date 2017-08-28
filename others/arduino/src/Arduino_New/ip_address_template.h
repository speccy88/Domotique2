static byte myip[] = { 192,168,0,100 };                                                // Static IP Address (Set this to whatever IP you want
static byte gwip[] = { 192,168,0,1 };                                                  // Static Gateway IP Address
static byte subnet[] = { 255,255,255,0 };                                              // Static Subnet Mask
static byte mymac[] = { 0xFF,0xFF,0xFF,0xFF,0xFF,0x01 };                               // Static MAC Address - MUST NOT have 2 identical MAC address on same network

//Disable unused functions to free up some memory on your Arduino project
  #define enable_base         //8% memory use --> digital in/out, analog, pwm, frequency count, tone
  #define enable_temp         //15% memory use --> Works for DHT22 ONLY
  //#define enable_baro         //19% memory use --> Works for I2C address 0x76, code modifications needed for SPI (see github library) - Select BME or BMP in function calls
  //#define enable_expander     //2% memory use  --> Works for PCF8575/C ONLY, code modifications needed for PCF8574 (see github library)
  //#define enable_stgc         //6% memory use
  //#define enable_oled         //8% memory use  --> Works for SSD1306 ONLY using text library only

  #ifdef enable_stgc
    #define campass_enable
  #endif

  #ifdef enable_baro
    #define baro_bmp          //Working
    //#define baro_bme          //Untested

    #define baro_pres         //Working
    #define baro_temp         //Working
    #define baro_alt          //Working

    #ifdef baro_bme
      //#define baro_hum        //Untested
      //#define baro_hi         //Untested
    #endif
  #endif
