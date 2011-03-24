#ifndef NETWORK_H
#define NETWORK_H

#define MULTICAST_ADDR "234.98.98.98"
#define MULTICAST_TTL 2
#define TCPPORT 7898
#define UDPPORT 7899
#define PACKETSIZE 4096
#define MSG_FTREQ 0x01
#define MSG_FT 0x02
#define MSG_FTCOMPLETE 0x03
#define MSG_STREAMREQ 0x04
#define MSG_STREAMPAUSE 0x05
#define MSG_STREAMCOMPLETE 0x06
#define MSG_AUDIO 0x07
#define MSG_CHAT 0x08
#define MSG_LISTREQ 0x09
#define MSG_LIST 0x0A
#define MSG_ACK  0x0B
void WinsockInit();
void WinsockCleanup();
void ProcessUDPPacket(char* packet);
void ProcessTCPPacket(char* packet);
void mkPacket(char* buf, char msgtype, unsigned short packetSize,
                                        char destClient, char* data);
unsigned short dataLength(char* buf);
#endif
