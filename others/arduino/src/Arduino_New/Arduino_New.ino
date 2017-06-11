#include <Arduino.h>   
#include <EtherCard.h>                                                                 // Ethernet controller library
#include <IPAddress.h>                                                                 // IPAddress controller library
#include <avr/wdt.h>                                                                   // Watchdog controller library

// Modify the file ip_address_template.h for your own setup and rename to ip_address.h
// This ensures you personnal data such as IP addresses doesn't get on github to become public
#include "ip_address.h"

#include "digital.h"
#include "temp.h"
#include "pressure.h"
#include "expander.h"
#include "stgc.h"
#include "OLED128x64.h"

#define BUFLEN 32                                                                      // Define length of the reply string
#define ETHERBUFLEN 100                                                                // tcp/ip send and receive buffer

#define DEBUG                                                                          // Comment this line to remove all serial data to free up some memory

#include "error_codes.h"                                                               // Includes error codes and pins definitions (min/max pin numbers)

static int Ether_cspin = 10;                                                           // Chip select pin numper for ethernet shield
static int ListenPort = 5000;                                                          // Static port to listen

byte Ethernet::buffer[ETHERBUFLEN];                                                    // tcp/ip send and receive buffer

int level = 0;
int pin = 0;

int delimiters[10];
int delimiter_qty = 0;
String commands[10];

char replyUDP[BUFLEN];
int SourcePort;                                                                                    // Source port message came from in order to reply

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
  command.toLowerCase();
  int reply[8];
  int level = 0;
  int pin = 0;
  // FIRST COMPARISON ALWAYS FALSE
  // THIS WILL ALLOW TO DISABLE SOME SUBROUTINES
  if(1 == 0)
    pin = commands[1].toInt();

  // DIGITAL INPUTS AND OUTPUTS
  #ifdef enable_digital
    // WRITE DIGITAL OUTPUT PIN
    else if(command == "write")
    {
      pin = commands[1].toInt();
      level = commands[2].toInt();
  
      reply[0] = digital.SET(pin, level);
      
      sprintf(str, "%d", reply[0]);
    }
    // READING DIGITAL INPUT PIN
    else if(command == "read")
    {
      pin = commands[1].toInt();
      
      reply[0] = digital.READ(pin);
      
      sprintf(str, "%d", reply[0]);
    }
  #else
    else if(command == "write" || command == "read")
    {
      reply[0] = ERROR_COMMAND_NOT_ACTIVATED;
      sprintf(str, "%d", reply[0]);
    }
  #endif
  
  //ANALOG INPUTS
  #ifdef enable_analogin
    // READING ANALOG INPUT PIN
    else if(command == "analog")
    {
      //Convert ascii pin number to numerical pin number
      int aPosition = commands[1].indexOf('a');
      //int APosition = commands[1].indexOf('A');
      if(aPosition == 0)// || APosition == 0)
      {
        commands[1].remove(0, 1);                                                                  // Remove first letter of the analog pin
        pin = commands[1].toInt() + 14;
      }
      else
        pin = commands[1].toInt();
  
      reply[0] = digital.ANALOG(pin);
      sprintf(str, "%d", reply[0]);
    }
  #else
    else if(command == "analog")
    {
      reply[0] = ERROR_COMMAND_NOT_ACTIVATED;
      sprintf(str, "%d", reply[0]);
    }
  #endif
        
  // ANALOG OUTPUTS (PWM)
  #ifdef enable_analogout
    // WRITE ANALOG OUTPUT PIN (PWM)
    // Command[3] should be equal to maximum of your scale (100 for percent, 255 8bits, etc.)
    else if(command == "pwm")
    {
      pin = commands[1].toInt();
      level = commands[2].toInt();
  
      reply[0] = digital.PWM(pin, (level*commands[3].toInt()));                                    // Convert data from 0-100% to 0-255
      sprintf(str, "%d", reply[0]);
    }
  #else
    else if(command == "pwm")
    {
      reply[0] = ERROR_COMMAND_NOT_ACTIVATED;
      sprintf(str, "%d", reply[0]);
    }
  #endif

  // FREQUENCY COUNTER
  #ifdef enable_frequency    
    // READ PIN FREQUENCY in Hz
    else if(command == "freq")
    {
      pin = commands[1].toInt();
  
      wdt_reset();                                                                                 // Exceptionally reset watchdog before going into the frequency count function to prevent unwanted reset
                                                                                                   // Each frequency count times out after 2s, so should be ok 
      reply[0] = digital.FREQ(pin);
      sprintf(str, "%d", reply[0]);
    }
  #else
    else if(command == "freq")
    {
      reply[0] = ERROR_COMMAND_NOT_ACTIVATED;
      sprintf(str, "%d", reply[0]);
    }
  #endif
    
  // TONE GENERATOR
  #ifdef enable_tone
    // MAKE TONE ON DIGITAL PIN
    else if(command == "tone")
    {
      pin = commands[1].toInt();
      level = commands[2].toInt();
  
      reply[0] = digital.TONE(pin, level, commands[3].toInt(), commands[4]);
      sprintf(str, "%d", reply[0]);
    }
  #else
    else if(command == "tone")
    {
      reply[0] = ERROR_COMMAND_NOT_ACTIVATED;
      sprintf(str, "%d", reply[0]);
    }
  #endif

  // GPIO EXPANDER
  #ifdef enable_expander
    // GPIO EXPANDER BASED ON PCF8575 (or PCF8574 with some modifications)
    else if(command == "expwrite")
    {
      pin = commands[1].toInt();
      level = commands[2].toInt();
  
      reply[0] = expander.SET(pin, level, commands[3].toInt());
      sprintf(str, "%d", reply[0]);
    }
    else if(command == "expread")
    {
      pin = commands[1].toInt();
        
      reply[0] = expander.READ(pin, commands[3].toInt());
      sprintf(str, "%d", reply[0]);
    }
  #else
    else if(command == "tone")
    {
      reply[0] = ERROR_COMMAND_NOT_ACTIVATED;
      sprintf(str, "%d", reply[0]);
    }
  #endif

  // TEMPERATURE/HUMIDITY
  #ifdef enable_temp
    // READ TEMPERATURE, HUMIDITY AND HEAT INDEX USING DHT22
    else if(command == "temp")
    {
      pin = commands[1].toInt();
      
      reply[0] = temp.TEMPERATURE(pin, commands[2], 1);
      reply[1] = temp.TEMPERATURE(pin, commands[2], 2);
      reply[2] = temp.TEMPERATURE(pin, commands[2], 3);
      sprintf(str, "%d,%d,%d", reply[0], reply[1], reply[2]);
    }
  #else
    else if(command == "temp")
    {
      reply[0] = ERROR_COMMAND_NOT_ACTIVATED;
      sprintf(str, "%d", reply[0]);
    }
  #endif

  // BAROMETRIC PRESSURE
  #ifdef enable_baro
    // READ BAROMETRIC PRESSURE USING BMP280
    else if(command == "pres")
    {
      pin = commands[1].toInt(); //pin is the I2C address or CS pin for the SPI version
      level = commands[2].toInt(); //level determines I2C=0 or SPI=1
  
      reply[0] = pressure.PRES(pin);
      sprintf(str, "%d", reply[0]);
    }
  #else
    else if(command == "pres")
    {
      reply[0] = ERROR_COMMAND_NOT_ACTIVATED;
      sprintf(str, "%d", reply[0]);
    }
  #endif
  
  // STGC DECODER
  #ifdef enable_stgc
    // READ SINGLE TRACK GRAY CODE ENCODER
    else if(command == "stgc")
    {
      reply[0] = STGC.READ(commands[1].toInt(), commands[2].toInt(), commands[3].toInt(), commands[4].toInt(), commands[5].toInt());
      sprintf(str, "%d", reply[0]);
    }
  #else
    else if(command == "stgc")
    {
      reply[0] = ERROR_COMMAND_NOT_ACTIVATED;
      sprintf(str, "%d", reply[0]);
    }
  #endif

  // OLED DISPLAY
  #ifdef enable_oled
    // OLED DISPLAY USING SSD1309 128x64 (maybe 32 later)
    else if(command == "oled")
    {
      reply[0] = OLED.SEND64(commands[1], commands[2], commands[3], commands[4], commands[5]);
      sprintf(str, "%d", reply[0]);
    }
  #else
    else if(command == "oled")
    {
      reply[0] = ERROR_COMMAND_NOT_ACTIVATED;
      sprintf(str, "%d", reply[0]);
    }
  #endif
  
  else
  {
    reply[0] = ERROR_UNDEFINED_COMMAND;
    sprintf(str, "%d", reply[0]);
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
