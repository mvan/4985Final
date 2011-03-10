#ifndef NETWORK_H
#define NETWORK_H
#include <Winsock2.h>
#include "helpers.h"
#define TCPPORT 7898
#define UDPPORT 7899
#define BUFSIZE 4096
#define SOCK_ERROR 1
#define RD_ERROR 2
#define WR_ERROR 3
#define EVT_ERROR 4
#define THREAD_ERROR 5
#define ALLOC_ERROR 6
#define FILE_ERROR 7

void WinsockInit();
void WinsockCleanup();
void WSAError(int error);

void TCPSocket_Init(SOCKET* sock);
void TCPSocket_Bind(SOCKET* sock, int PortNo);
void TCPSocket_Listen(SOCKET* sock);
void TCPSocket_Connect(SOCKET* sock, int PortNo, char* serv_addr);
void UDPSocket_Init(SOCKET* sock);
void UDPSocket_Bind(SOCKET* sock, int PortNo);
#endif
