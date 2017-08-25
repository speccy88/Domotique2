#include <Arduino.h>   
#include <EtherCard.h>                                                                 // Ethernet controller library
#include <IPAddress.h>                                                                 // IPAddress controller library
#include <avr/wdt.h>                                                                   // Watchdog controller library
#include "parse.h"

// Modify the file ip_address_template.h for your own setup and rename to ip_address.h
// This ensures you personnal data such as IP addresses doesn't get on github to become public
#include "ip_address.h"

#include "io.h"

#define UDP_REPLY_BUFLEN 50                    // Define length of the reply string
#define UDP_RECV_BUFLEN  100                   // tcp/ip send and receive buffer

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
  #ifdef DEBUG
    Serial.println("<- DATA PARSED");
  #endif
  
  callSubfunction(commands);
  UDPreply();
}

void callSubfunction(char (*commands)[COMMAND_LENGTH])
{
  char* command = commands[0];

  io = new ioclass(commands);    //pass commands buffer to temp class
  io->process(UDP_Reply_Buffer);  //process the commands and put results in reply buffer
}

void UDPreply()
{
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

  #ifdef DEBUG
    Serial.begin(9600);
    Serial.println("[START]");
  #endif
  
  ether.begin(sizeof Ethernet::buffer, mymac, Ether_cspin);
  ether.staticSetup(myip, gwip, subnet);

  ether.printIp("Arduino IP: ", ether.myip);

  ether.udpServerListenOnPort(&udpReceive, ListenPort);
}

void loop()
{
  ether.packetLoop(ether.packetReceive());
}
