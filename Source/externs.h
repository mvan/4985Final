#ifndef EXTERNS_H
#define EXTERNS_H
#include <WinSock2.h>
#include "network.h"
#include "buffer.h"
#include "stats.h"
extern PSOCK sockets;
extern HWND hwndDlg, hwnd;
extern int numPackets, packetSize;
extern struct sockaddr_in udp_serv;
extern struct hostent	*udp_host;
extern char FileName[MAX_PATH], FilePath[MAX_PATH];
extern PBUFFER head, tail;
extern size_t numBuffers;
extern CRITICAL_SECTION critsect;
extern BOOL sendfile;
extern STATS stats;
extern SOCKET UDPsocket;
extern SOCKET TCPsocket;
extern HANDLE file;
void ResetGlobals();
#endif