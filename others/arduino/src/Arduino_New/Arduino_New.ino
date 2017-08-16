#include <Arduino.h>   
#include <EtherCard.h>                                                                 // Ethernet controller library
#include <IPAddress.h>                                                                 // IPAddress controller library
#include <avr/wdt.h>                                                                   // Watchdog controller library
#include "parse.h"


// Modify the file ip_address_template.h for your own setup and rename to ip_address.h
// This ensures you personnal data such as IP addresses doesn't get on github to become public
#include "ip_address.h"

#include "digital.h"
#include "temp.h"
#include "pressure.h"
#include "expander.h"
#include "stgc.h"
#include "OLED.h"

#define UDP_REPLY_BUFLEN 32                    // Define length of the reply string
#define UDP_RECV_BUFLEN  100                   // tcp/ip send and receive buffer

//#define DEBUG                                  // Comment this line to remove all serial data to free up some memory

#include "error_codes.h"                       // Includes error codes and pins definitions (min/max pin numbers)

static int Ether_cspin = 10;                   // Chip select pin numper for ethernet shield
static int ListenPort = 5000;                  // Static port to listen

byte Ethernet::buffer[UDP_RECV_BUFLEN];
char UDP_Reply_Buffer[UDP_REPLY_BUFLEN];
int SourcePort;


//callback that prints received packets to the serial port
void udpReceive(uint16_t dest_port, uint8_t src_ip[IP_LEN], uint16_t src_port, const char *data, uint16_t len)
{
  IPAddress src(src_ip[0],src_ip[1],src_ip[2],src_ip[3]);
  char (*commands)[COMMAND_LENGTH];

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
    Serial.println("<- DATA RECEIVED");
  #endif

  
  commands = parseData(data);
  Serial.println("<- DATA PARSED");
  
  
  callSubfunction(commands);
  UDPreply();
  
}



void callSubfunction(char (*commands)[COMMAND_LENGTH])
{
  
  char* command = commands[0];
  int reply[8];

  int pin;
  int level;
  // FIRST COMPARISON ALWAYS FALSE
  // THIS WILL ALLOW TO DISABLE SOME SUBROUTINES
  if(false)
    delayMicroseconds(0);
  #ifdef enable_temp
  else if(strcmp(command,"temp") == 0)
  {
    Serial.println("BEFORE TEMP");
    //Serial.println(str);
    temp = new tempclass(commands);    //pass commands buffer to temp class
    temp->process(UDP_Reply_Buffer);   //process the commands and put results in reply buffer
  }
  #endif
  // DIGITAL INPUTS AND OUTPUTS
  #ifdef enable_digital
    // WRITE DIGITAL OUTPUT PIN
    else if(strcmp(command,"write") == 0)
    {
      pin = atoi(commands[1]);
      level = atoi(commands[2]);

      pinMode(pin, OUTPUT);
      digitalWrite(pin, level);

      strcpy (UDP_Reply_Buffer,"OK");
    }
    // READING DIGITAL INPUT PIN
    else if(strcmp(command,"read") == 0)
    {
      pin = atoi(commands[1]);
      pinMode(pin, INPUT_PULLUP);
      level = digitalRead(pin);

      char read_buf[2];
      read_buf[0] = level+48;
      read_buf[1] = 0;

      strcpy (UDP_Reply_Buffer,read_buf);
    }
  #endif
  else
  {
    Serial.println("FAIL TO FIND FUNCTION");
    reply[0] = ERROR_UNDEFINED_COMMAND;
    //sprintf(str, "%d", reply[0]);
  }  
}

void UDPreply()
{
  //strcpy(UDP_Reply_Buffer, str);

  #ifdef DEBUG
    Serial.print("Reply: ");
    Serial.println(UDP_Reply_Buffer);
    Serial.print("Reply Length: ");
    Serial.println(strlen(UDP_Reply_Buffer));
    Serial.println("Watchdog Reset...");
    Serial.println("Command Completed...");
    Serial.println("-----------------------------------------------------");
  #endif

  ether.makeUdpReply(UDP_Reply_Buffer, strlen(UDP_Reply_Buffer), SourcePort);
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
