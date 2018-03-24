#define ETH_CS_PIN       10            // Chip select pin numper for ethernet shield
#define UDP_REPLY_BUFLEN 50            // Define length of the reply string
#define UDP_RECV_BUFLEN  200           // tcp/ip send and receive buffer
#define UDP_LISTEN_PORT  5000          // Static port to listen

void udpReceive(uint16_t dest_port, uint8_t src_ip[IP_LEN], uint16_t src_port, const char *data, uint16_t len);
void UDPreply(uint16_t src_port);

byte Ethernet::buffer[UDP_RECV_BUFLEN];
char UDP_Reply_Buffer[UDP_REPLY_BUFLEN];
