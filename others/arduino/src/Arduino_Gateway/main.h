#include "defines.h"

void udpReceive(uint16_t dest_port, uint8_t src_ip[IP_LEN], uint16_t src_port, const char *data, uint16_t len);
void UDPreply(uint16_t src_port);

byte Ethernet::buffer[UDP_RECV_BUFLEN];
char UDP_Reply_Buffer[UDP_REPLY_BUFLEN];
