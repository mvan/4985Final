#ifndef NETWORK_H
#define NETWORK_H
#include <Winsock2.h>
#include "helpers.h"
#define DEFAULT_PORT 7898
#define BUFSIZE 65535
#define SOCK_ERROR 1
#define RD_ERROR 2
#define WR_ERROR 3
#define EVT_ERROR 4
#define THREAD_ERROR 5
#define ALLOC_ERROR 6
#define FILE_ERROR 7
#define WM_TCPSOCKET (WM_USER+1)
#define WM_UDPSOCKET (WM_USER+2)

typedef struct SOCKET_INFO{

   SOCKET sock;
   DWORD NumRcv;
   CHAR buf[BUFSIZE];
   WSABUF dbuf;
   SOCKET_INFO* next;

} SOCK, * PSOCK;

void TCP_Server(HWND hwnd, int PortNo);
void UDP_Server(HWND hwnd, int PortNo);
void TCP_Client(char* serv_addr, int PortNo);
void Call_TCP_Cl(HWND hwndDlg) ;
void UDP_Client(char* serv_addr, int PortNo);
void Call_UDP_Cl(HWND hwndDlg);
int TCP_Send(SOCKET s, int PacketSize, char* data);
int UDP_Send(SOCKET s, struct sockaddr *sa, int PacketSize, char* data);
void Add_Sock(SOCKET s);
PSOCK Find_Sock(SOCKET s);
void Remove_Sock(SOCKET s);
void WinsockInit();
void WinsockCleanup(SOCKET socket);
void WSAError(int error);
void TCPSocket_Init(SOCKET* sock);
void TCPSocket_Listen(SOCKET* sock, int PortNo);
void TCPSocket_Connect(SOCKET* sock, int PortNo, char* serv_addr);
void UDPSocket_Init(SOCKET* sock);
void UDPSocket_Bind(SOCKET* sock, int PortNo);
DWORD WINAPI tcp_thrd(LPVOID param);
DWORD WINAPI udp_thrd(LPVOID param);
#endif