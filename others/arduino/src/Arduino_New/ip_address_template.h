static byte myip[] = { 192,168,0,100 };                                                // Static IP Address (Set this to whatever IP you want
static byte gwip[] = { 192,168,0,1 };                                                  // Static Gateway IP Address
static byte subnet[] = { 255,255,255,0 };                                              // Static Subnet Mask
static byte mymac[] = { 0xFF,0xFF,0xFF,0xFF,0xFF,0x01 };                               // Static MAC Address - MUST NOT have 2 identical MAC address on same network

//Disable unused functions to free up some memory on your Arduino project
  //#define enable_digital      //1% memory use
  //#define enable_analogin     //0% memory use
  //#define enable_analogout    //1% memory use
  //#define enable_frequency    //1% memory use
  //#define enable_tone         //4% memory use
  //#define enable_temp         //14% memory use
  //#define enable_baro         //16% memory use
  //#define enable_expander     //2% memory use
  //#define enable_stgc         //1% memory use
  //#define enable_oled         //4% memory use
