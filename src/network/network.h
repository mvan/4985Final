#ifndef NETWORK_H
#define NETWORK_H

#define MULTICAST_ADDR "234.98.98.98"
#define MULTICAST_TTL 2
#define TCPPORT 7898
#define UDPPORT 7899
#define PACKETSIZE 4096
void WinsockInit();
void WinsockCleanup();
#endif