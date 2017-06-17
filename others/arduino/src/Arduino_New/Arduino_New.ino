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

#define BUFLEN 32                                                                      // Define length of the reply string
#define ETHERBUFLEN 100                                                                // tcp/ip send and receive buffer

#define DEBUG                                                                          // Comment this line to remove all serial data to free up some memory

#include "error_codes.h"                                                               // Includes error codes and pins definitions (min/max pin numbers)

#define NUMBER_OF_COMMANDS 10
#define COMMAND_LENGTH 16

char **commands;

static int Ether_cspin = 10;                                                           // Chip select pin numper for ethernet shield
static int ListenPort = 5000;                                                          // Static port to listen

byte Ethernet::buffer[ETHERBUFLEN];                                                    // tcp/ip send and receive buffer

int level = 0;
int pin = 0;

char replyUDP[BUFLEN];
int SourcePort;                                                                        // Source port message came from in order to reply

char str[BUFLEN];

//skills
tempclass temp = tempclass(commands);


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

  Serial.println("DATA RECEIVED");
  commands = parseData(data);
  Serial.println("DATA PARSED");
  Serial.println(commands[0]);

  Serial.println(strcmp (commands[0],"temp") == 0);
  
  callSubfunction();
  
  UDPreply();
  
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
    pin = atoi(commands[1]);
  else if(strcmp(commands[0],"temp") == 0)
  {
    Serial.println("BEFORE TEMP");
    Serial.println(str);
    temp.process(str);
  }
  else
  {
    Serial.println("FAIL TO FIND FUNCTION");
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
