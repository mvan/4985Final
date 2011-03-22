#ifndef NETWORK_H
#define NETWORK_H

#define MULTICAST_ADDR "234.98.98.98"
#define MULTICAST_TTL 2
#define TCPPORT 7898
#define UDPPORT 7899
#define PACKETSIZE 4096
#define MSG_FT 0x01
#define MSG_FTCOMPLETE 0x02
#define MSG_AUDIO 0x03
#define MSG_CHAT 0x04
#define MSG_LIST 0x05
void WinsockInit();
void WinsockCleanup();
void mkPacket(char* buf, char msgtype, char srcClient,
              char destClient, char* data);
void ProcessUDPPacket(char* packet);
void ProcessTCPPacket(char* packet);
#endif
