#include <EtherCard.h>                 // Ethernet controller library
#include <IPAddress.h>                 // IPAddress controller library 
#include <avr/wdt.h>                   // Watchdog controller library
#include "parse.h"
#include "main.h"
#include "error_codes.h"               // Includes error codes and pins definitions (min/max pin numbers)
#include "commands_arduino_basic.h"

//Command table
#define NUMBER_OF_COMMANDS 10
#define COMMAND_LENGTH 22
char command_table[NUMBER_OF_COMMANDS][COMMAND_LENGTH];
char (*command_ptr)[COMMAND_LENGTH] = (&command_table)[COMMAND_LENGTH];

//Commands
Commands_Arduino_Basic arduinoBasic = Commands_Arduino_Basic();

#define DEBUG
//#define WDT

const byte ETH_IP_ADDRESS[] = { 192,168,1,95 };                     // Static IP Address (Set this to whatever IP you want
const byte ETH_GATEWAY[]    = { 192,168,1,1 };                      // Static Gateway IP Address
const byte ETH_SUBNET[]     = { 255,255,255,0 };                    // Static Subnet Mask
const byte MAC_ADDRESS[]    = { 0xFF,0xFF,0xFF,0xFF,0xFF,0x02 };    // Static MAC Address - MUST NOT have 2 identical MAC address on same network

//First time setup
void setup()
{
  #ifdef WDT
    wdt_enable(WDTO_8S);
  #else
    wdt_disable();
  #endif

  #ifdef DEBUG
    Serial.begin(9600);
    Serial.println("[START]");
  #endif
  
  ether.begin(sizeof Ethernet::buffer, MAC_ADDRESS, ETH_CS_PIN);
  ether.staticSetup(ETH_IP_ADDRESS, ETH_GATEWAY, ETH_SUBNET);

  #ifdef DEBUG
  ether.printIp("Arduino IP: ", ether.myip);
  #endif

  ether.udpServerListenOnPort(&udpReceive, UDP_LISTEN_PORT);
}

//Poll for a new packet
void loop()
{
  ether.packetLoop(ether.packetReceive());
  
  #ifdef WDT
    wdt_reset();
  #endif
}

//New UDP packet received
void udpReceive(uint16_t dest_port, uint8_t src_ip[IP_LEN], uint16_t src_port, const char *data, uint16_t len)
{
  IPAddress src(src_ip[0],src_ip[1],src_ip[2],src_ip[3]);

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
  
  parseData(data);
 
  #ifdef DEBUG
    Serial.println("<- DATA PARSED");
    Serial.println(command_ptr[0]);
    Serial.println(command_ptr[1]);
    Serial.println(command_ptr[2]);
  #endif
  //Call sub-functions
  arduinoBasic.Process(UDP_Reply_Buffer);
  
  //Reply
  UDPreply(src_port);
}

void UDPreply(uint16_t src_port)
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

  ether.makeUdpReply(UDP_Reply_Buffer, strlen(UDP_Reply_Buffer), src_port);
}
