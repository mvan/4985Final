#ifndef STATS_H
#define STATS_H
#include <WinSock2.h>
#define WM_STATS (WM_USER+3)
#define STATCONNECT 1
#define STATDISCONNECT 0
#define STATTCP 999
#define STATUDP 998

typedef struct _stats_ {
	int TOTALUDPPACKETS;
	int TOTALTCPPACKETS;
	int TOTALDATA;
	int PACKETSIZE;
	int TYPE;
	int STATE;
	DWORD START_TIME;
	DWORD CURRENT_TIME;
} STATS, *PSTATS;
void Serv_Stats();
void Cli_Stats();
int TransferRate(DWORD TotalTime);
int avgPacketTransferRate(DWORD TotalTime, int NumPackets);
DWORD WINAPI stats_thread(LPVOID param) ;
#endif