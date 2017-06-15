static byte myip[] = { 192,168,0,100 };                                                // Static IP Address (Set this to whatever IP you want
static byte gwip[] = { 192,168,0,1 };                                                  // Static Gateway IP Address
static byte subnet[] = { 255,255,255,0 };                                              // Static Subnet Mask
static byte mymac[] = { 0xFF,0xFF,0xFF,0xFF,0xFF,0x01 };                               // Static MAC Address - MUST NOT have 2 identical MAC address on same network

//Disable unused functions to free up some memory on your Arduino project
#define enable_digital      //1% memory use
#define enable_analogin     //2% memory use
#define enable_analogout    //2% memory use
#define enable_frequency    //2% memory use
#define enable_tone         //6% memory use
#define enable_temp         //14% memory use
#define enable_baro         //18% memory use
#define enable_expander     //3% memory use
#define enable_stgc         //2% memory use
#define enable_oledascii    //4% memory use
#define enable_oledfull     //34% memory use
