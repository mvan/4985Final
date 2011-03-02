#include <WinSock2.h>
#include "network.h"
#include "buffer.h"
#include "stats.h"
PSOCK sockets;
struct sockaddr_in udp_serv;
struct hostent	*udp_host = 0;
HWND hwnd, hwndDlg;
char FileName[MAX_PATH], FilePath[MAX_PATH];
int numPackets = 0, packetSize = 0;
PBUFFER head = 0, tail = 0;
size_t numBuffers = 0;
CRITICAL_SECTION critsect;
BOOL sendfile = FALSE;
STATS stats = {0,0,0,0,0,0,0,0};
SOCKET UDPsocket, TCPsocket;
HANDLE file;


void ResetGlobals() {

	ZeroMemory(&udp_serv, sizeof(struct sockaddr_in));
	if(udp_host != NULL) {
		ZeroMemory(udp_host, sizeof(struct hostent));
	}
	ZeroMemory(FilePath, MAX_PATH);
	ZeroMemory(FileName, MAX_PATH);
	numPackets = 0;
	packetSize = 0;
	head = 0;
	tail = 0;
	numBuffers = 0;
	sendfile = FALSE;
	ZeroMemory(&stats, sizeof(stats));
	UDPsocket = 0;
	TCPsocket = 0;

}