#include <EtherCard.h>
#include <IPAddress.h>

static byte myip[] = { 192,168,2,199 };
static byte gwip[] = { 192,168,2,1 };

static byte mymac[] = { 0x70,0x69,0x69,0x2D,0x30,0x31 };

byte Ethernet::buffer[500]; // tcp/ip send and receive buffer

//callback that prints received packets to the serial port
void udpReceive(uint16_t dest_port, uint8_t src_ip[IP_LEN], uint16_t src_port, const char *data, uint16_t len)
{
  IPAddress src(src_ip[0],src_ip[1],src_ip[2],src_ip[3]);
  
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
  switch(command) 
  {
   case 'W':
   {
    Serial.println("Command: Write digital output");
    params = strtok(0, ":");

    int pin = atoi(params);
    params = strtok(0, ":");

    int level = atoi(params);
    Serial.print("pin:");
    Serial.println(pin);
    Serial.print("level:");
    Serial.println(level);
      
    pinMode(pin, OUTPUT);
    digitalWrite(pin, level);
   }
   break;
  
   case 'R':
    Serial.println("TODO");
    break; 

   default:
    Serial.println("Good");
  }
}



void setup()
{
  Serial.begin(9600);
  Serial.println("[START]");

  ether.begin(sizeof Ethernet::buffer, mymac, 8);
  ether.staticSetup(myip, gwip);

  ether.printIp("Arduino IP: ", ether.myip);

  ether.udpServerListenOnPort(&udpReceive, 5000);
}

void loop()
{
  ether.packetLoop(ether.packetReceive());
}
