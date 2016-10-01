#include <EtherCard.h>
#include <IPAddress.h>

static byte myip[] = { 192,168,0,6 };
static byte gwip[] = { 192,168,0,2 };
static byte subnet[] = { 255,255,255,0 };

static byte mymac[] = { 0x70,0x69,0x69,0x2D,0x30,0x31 };

byte Ethernet::buffer[500]; // tcp/ip send and receive buffer

    int level = 0;
    int pin = 0;
  
    char pinstatus[] = "";
    int nSourcePort = 0;
    
    String str=String(level);

    char read_write;

//callback that prints received packets to the serial port
void udpReceive(uint16_t dest_port, uint8_t src_ip[IP_LEN], uint16_t src_port, const char *data, uint16_t len)
{
  IPAddress src(src_ip[0],src_ip[1],src_ip[2],src_ip[3]);

  nSourcePort = dest_port;
  
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
   case 'A':
   case 'a':
   {
     Serial.println("Command: Read analog input");

     level = analogRead(pin);
   }
   break;
   
   case 'S':
   case 's':
   {
     Serial.print("Command: Setup pin ");

     params = strtok(0, ":");
     level = atoi(params);

     switch(level)
     {
       case 0:
         {
           Serial.println("digital Output");
           pinMode(pin, OUTPUT);
         }
       break;
       case 1:
         {
           Serial.println("digital Input");
           pinMode(pin, INPUT);
         }
       break;
       case 2:
         {
           Serial.println("analog Input");
           //TODO ANALOG INPUT
         }
       break;
       case 3:
         {
           Serial.println("PWM Output");
           //TODO PWM OUTPUT
         }
       break;
     }
   }
   break;
   
   case 'W':
   case 'w':
   {
     Serial.println("Command: Write digital pin");

     params = strtok(0, ":");
     level = atoi(params);
      
     //pinMode(pin, OUTPUT);
     digitalWrite(pin, level);

     str=String(digitalRead(pin));
   }
   break;
  
   case 'R':
   case 'r':
   {
     Serial.println("Command: Read digital pin");

     level = digitalRead(pin);   // read the input pin
    
     str=String(level);
   }
   break; 

   default:
     Serial.println("Good");
     break;
  }
  
  read_write = command;
  str.toCharArray(pinstatus,2);

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

  ether.makeUdpReply(pinstatus, sizeof(pinstatus), nSourcePort);
}
