#include <EtherCard.h>
#include <IPAddress.h>

static byte myip[] = { 192,168,0,6 };
static byte gwip[] = { 192,168,0,2 };
static byte subnet[] = { 255,255,255,0 };

//static byte mymac[] = { 0x70,0x69,0x69,0x2D,0x30,0x31 };
static byte mymac[] = { 0xFF,0xFF,0xFF,0xFF,0xFF,0x01 };

byte Ethernet::buffer[500]; // tcp/ip send and receive buffer

    int level = 0;
    int pin = 0;
  
    char replyUDP[] = "";
    int SourcePort = 0;
    
    String str=String(level);

//callback that prints received packets to the serial port
void udpReceive(uint16_t dest_port, uint8_t src_ip[IP_LEN], uint16_t src_port, const char *data, uint16_t len)
{
  IPAddress src(src_ip[0],src_ip[1],src_ip[2],src_ip[3]);

  SourcePort = dest_port;
  
  Serial.print("Arduino Port: ");
  Serial.println(dest_port);
  Serial.print("Raspberry Pi Port: ");
  Serial.println(src_port);
  
  
  Serial.print("Raspberry Pi IP: ");
  ether.printIp(src_ip);
  Serial.println("");
  Serial.print("Payload:");
  Serial.println(data);

  processData((char*)data);
}

void processData(char *data)
{ 
  char command = *strtok(data, ":");
  char* params;
  
  params = strtok(0, ":");
  pin = atoi(params);
  
  switch(command) 
  {
  //***********************************************************
  //READ Analog pin
  //Parameter: A:x 
  //  Where x = pin number (A0 to A5 or A7 on nano))
  //    Pin numbers can also be used (14 to 19 or 21 on nano))
  //***********************************************************
   case 'A':
   case 'a':
   {
     Serial.println("Command: Read analog input");

     level = analogRead(pin);
     
     str=String(level);
   }
   break;

  //***********************************************************
  //Read digital input
  //Parameter: r:x 
  //  Where x = pin number (0 to 13, A0 to A5)
  //***********************************************************
   case 'R':
   case 'r':
   {
     Serial.println("Command: Read digital pin");

     pinMode(pin, INPUT);
     level = digitalRead(pin);                                  // read the input pin
    
     str=String(level);
   }
   break;
   
  //***********************************************************
  //WRITE digital output
  //Parameter: W:x:y 
  //  Where x = pin number (0 to 19 or 21 on nano))
  //        y = Pin level
  //  Where levels are:
  //        0 = Digital Output LOW
  //        1 = Digital Output HIGH
  //***********************************************************
   case 'W':
   case 'w':
   {
     Serial.println("Command: Write digital pin");
    
    //Verify if pin was in input mode and if so, verify status to prevent change
     uint8_t bit = digitalPinToBitMask(pin);
     uint8_t port = digitalPinToPort(pin);
     volatile uint8_t *reg = portModeRegister(port);

     params = strtok(0, ":");
     level = atoi(params);
    
 /*    if (*reg & bit) 
     {
        // It's an output
        Serial.println("PIN was an output");
        pinMode(pin, OUTPUT);
        digitalWrite(pin, level);                               // write the output pin
     } 
     else 
     {
        // It's an input
        Serial.println("PIN was an input");
        if(level == digitalRead(pin))
        {
          pinMode(pin, OUTPUT);
          digitalWrite(pin, level);                             // write the output pin
        }
     }
*/
     pinMode(pin, OUTPUT);
     digitalWrite(pin, level);                               // write the output pin

     str=String(digitalRead(pin));                              // read the output pin
   }
   break;

  //***********************************************************
  //READ digital pin status
  //Parameter: r:x 
  //  Where x = pin number (0 to 13, A0 to A5)
  //***********************************************************
   case 'S':
   case 's':
   {
     Serial.println("Command: Read digital pin");

     level = digitalRead(pin);                                  // read the input pin
    
     str=String(level);
   }
   break; 

   default:
     Serial.println("Command: Invalid command received");
     break;
  }
  
  str.toCharArray(replyUDP,sizeof (str));

  Serial.print("Pin:");
  Serial.println(pin);
  Serial.print("Level:");
  Serial.println(level);
  
  Serial.println("Command Completed...");
  Serial.println("-----------------------------------------------------");
}

void setup()
{
  Serial.begin(9600);
  Serial.println("[START]");

  ether.begin(sizeof Ethernet::buffer, mymac, 8);
  ether.staticSetup(myip, gwip, subnet);

  ether.printIp("Arduino IP: ", ether.myip);

  ether.udpServerListenOnPort(&udpReceive, 5000);
}

void loop()
{
  ether.packetLoop(ether.packetReceive());

  ether.makeUdpReply(replyUDP, strlen(replyUDP), SourcePort);
}
