#include <Arduino.h>   
#include <EtherCard.h>                                                                 // Ethernet controller library
#include <IPAddress.h>                                                                 // IPAddress controller library
#include <avr/wdt.h>                                                                   // Watchdog controller library

#include "digital.h"
#include "temp.h"

#include "DHT.h"
#define DHTTYPE DHT22                                                                  // DHT 22  (AM2302), AM2321

#define BUFLEN 32                                                                      // Define length of the reply string

#define DEBUG

//Select ONLY one controller and comment the others
//#define House
#define Garage
//#define Pool

static int Ether_cspin = 10;                                                           // Chip select pin numper for ethernet shield
static int ListenPort = 5000;                                                          // Static port to listen

//Set IP address and MAC according to selected defined controller
#ifdef House
  static byte myip[] = { 192,168,0,6 };                                                // Static IP Address
  static byte mymac[] = { 0xFF,0xFF,0xFF,0xFF,0xFF,0x01 };                             // Static MAC Address - MUST NOT have 2 identical MAC address on same network
#endif
#ifdef Garage
  static byte myip[] = { 192,168,0,7 };                                                // Static IP Address
  static byte mymac[] = { 0xFF,0xFF,0xFF,0xFF,0xFF,0x02 };                             // Static MAC Address - MUST NOT have 2 identical MAC address on same network
#endif
#ifdef Pool
  static byte myip[] = { 192,168,0,8 };                                                // Static IP Address
  static byte mymac[] = { 0xFF,0xFF,0xFF,0xFF,0xFF,0x03 };                             // Static MAC Address - MUST NOT have 2 identical MAC address on same network
#endif

static byte gwip[] = { 192,168,0,2 };                                                  // Static Gateway IP Address
static byte subnet[] = { 255,255,255,0 };                                              // Static Subnet Mask

byte Ethernet::buffer[500];                                                           // tcp/ip send and receive buffer

int level = 0;
int pin = 0;

int delimiters[10];
int delimiter_qty = 0;
String commands[10];

char replyUDP[BUFLEN];
int SourcePort;                                                                        // Source port message came from in order to reply

char str[BUFLEN];

//callback that prints received packets to the serial port
void udpReceive(uint16_t dest_port, uint8_t src_ip[IP_LEN], uint16_t src_port, const char *data, uint16_t len)
{
  IPAddress src(src_ip[0],src_ip[1],src_ip[2],src_ip[3]);

  SourcePort = src_port;

  #ifdef DEBUG
    Serial.print("Arduino Port: ");
    Serial.println(dest_port);
    Serial.print("Raspberry Pi Port: ");
    Serial.println(src_port);
    
    Serial.print("Raspberry Pi IP: ");
    ether.printIp(src_ip);
    Serial.println("");
    Serial.print("Payload: ");
    Serial.println(data);
  #endif

  parseData(String(data));

  callSubfunction();
  
  UDPreply();
  
}

void parseData(String stringdata)
{  
  // Blank all string tables
  for(int i=0; i<=7; i++)
  {
    delimiters[i] = 0;
    commands[i] = "";
  }

  // Find all delimiters positions in received string
  delimiter_qty = 0;
  for(int i=0; i<=7; i++)
  {
    if(i==0)
      delimiters[i] = stringdata.indexOf(':');
    else
      delimiters[i] = stringdata.indexOf(':', delimiters[i-1]+1);
    
    if(delimiters[i]<=0)
      break;

    delimiter_qty += 1;
  }

  // Split and store data according to delimiters positions
  for(int i=0; i<=delimiter_qty; i++)
  {
    if(i==0)
      commands[i] = stringdata.substring(0, delimiters[i]);
    else if(i==delimiter_qty)
      commands[i] = stringdata.substring(delimiters[i-1]+1);
    else
      commands[i] = stringdata.substring(delimiters[i-1]+1, delimiters[i]);

    #ifdef DEBUG
      Serial.print("Command #");
      Serial.print(i+1);
      Serial.print(": ");
      Serial.println(commands[i]);
    #endif
  }
}

void callSubfunction()
{
  String command = commands[0];
  int reply[8];
  bool level = 0;
  int pin = 0;
  // WRITE DIGITAL OUTPUT PIN
  if(command == "write")
  {
    pin = commands[1].toInt();
    level = commands[2].toInt();

    if((3 <= pin <= 9) || (13 <= pin <= 19))                    //Discard SPI pins (2(Unknown), 10(CS), 11(MOSI), 12(MISO), 13(SCK)) - Also discard pins that are analog exclusive (20(A6), 21(A7))
      reply[0] = digital.SET(pin, level);
    else
      reply[0] = 99997;                                         //Reply "99997" means the pin number is not valid for digital read/write
    
    sprintf(str, "%d", reply[0]);
  }
  // READING DIGITAL INPUT PIN
  else if(command == "read")
  {
    pin = commands[1].toInt();

    if((3 <= pin <= 9) || (13 <= pin <= 19))                    //Discard SPI pins (2(Unknown), 10(CS), 11(MOSI), 12(MISO), 13(SCK)) - Also discard pins that are analog exclusive (20(A6), 21(A7))
      reply[0] = digital.READ(pin);
    else
      reply[0] = 99997;                                         //Reply "99997" means the pin number is not valid for digital read/write
    
    sprintf(str, "%d", reply[0]);
  }
  // READING ANALOG INPUT PIN
  else if(command == "analog")
  {
    //Convert ascii pin number to numerical pin number
    int aPosition = commands[1].indexOf('a');
    int APosition = commands[1].indexOf('A');
    if(aPosition == 0 || APosition == 0)
    {
      commands[1].remove(0, 1);
      pin = commands[1].toInt() + 14;
      Serial.println(pin);
    }
    else
    {
      pin = commands[1].toInt();
      Serial.println(pin);
    }

    if(0 <= pin <= 7)
      pin += 14;
    else if(14 <= pin <= 21)
      pin = pin;
    else
      pin = 0;

    if(14 <= pin <= 21)
      reply[0] = digital.ANALOG(pin);
    else
      reply[0] = 99998;                                         //Reply "99998" means the pin number is not valid for analog input
    
    sprintf(str, "%d", reply[0]);
  }
  else if(command == "temp")
  {
    pin = commands[1].toInt();
    reply[0] = temp.TEMPERATURE(pin, commands[2], 1);
    reply[1] = temp.TEMPERATURE(pin, commands[2], 2);
    reply[2] = temp.TEMPERATURE(pin, commands[2], 3);
    sprintf(str, "%d,%d,%d", reply[0], reply[1], reply[2]);
  }
  else
  {
    reply[0] = 99999;                                           //Reply "99999" means the command is not valid
  }  
}

void UDPreply()
{
  

  strcpy(replyUDP, str);

  #ifdef DEBUG
    Serial.print("Reply: ");
    Serial.println(replyUDP);
    Serial.print("Reply Length: ");
    Serial.println(strlen(replyUDP));
    Serial.println("Watchdog Reset...");
    Serial.println("Command Completed...");
    Serial.println("-----------------------------------------------------");
  #endif

  ether.makeUdpReply(replyUDP, strlen(replyUDP), SourcePort);
  wdt_reset();
}

void setup()
{
  wdt_disable();
  //wdt_enable(WDTO_8S);
  
  Serial.begin(9600);
  Serial.println("[START]");

  ether.begin(sizeof Ethernet::buffer, mymac, Ether_cspin);
  ether.staticSetup(myip, gwip, subnet);

  ether.printIp("Arduino IP: ", ether.myip);

  ether.udpServerListenOnPort(&udpReceive, ListenPort);
}

void loop()
{
  ether.packetLoop(ether.packetReceive());
}
