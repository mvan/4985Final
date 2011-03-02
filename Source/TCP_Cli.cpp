#include <WinSock2.h>
#include "resource.h"
#include "network.h"
#include "externs.h"
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: TCP_Client
--
-- DATE: Feb 19, 2011
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Duncan Donaldson
--
-- PROGRAMMER: Duncan Donaldson
--
-- INTERFACE: void TCP_Client(char* serv_addr, int PortNo, int NumPackets)
--							serv_addr	- the server address.
--							PortNo		- the port number to bind to.
--							NumPackets	- the number of packets to send.
--
-- RETURNS: void
--
-- NOTES:
-- Initializes a TCP client and sends FD_WRITE messages for the number of packets to send.
----------------------------------------------------------------------------------------------------------------------*/
void TCP_Client(char* serv_addr, int PortNo) {

	SOCKET sock;
	HANDLE hThread;
	DWORD tid;

	WinsockInit();
	TCPSocket_Init(&sock);
	TCPSocket_Connect(&sock, PortNo, serv_addr);
	WSAAsyncSelect(sock, hwnd, WM_TCPSOCKET, FD_WRITE|FD_CLOSE);
	TCPsocket = sock;

	if((hThread = CreateThread(NULL, 0, tcp_thrd, NULL, NULL, &tid)) == 0) {
		WSAError(THREAD_ERROR);
	}
}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: Call_TCP_Cl
--
-- DATE: Feb 19, 2011
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Duncan Donaldson
--
-- PROGRAMMER: Duncan Donaldson
--
-- INTERFACE: void Call_TCP_Cl(HWND hwndDlg)
--							hwndDlg - handle to the dialog window to get initialization params from.
--
-- RETURNS: void
--
-- NOTES:
-- gets TCP client initialization parameters, and calls the TCP_Client function with said parameters.
----------------------------------------------------------------------------------------------------------------------*/
void Call_TCP_Cl(HWND hwndDlg) {

		int portNo;
		BOOL retVal;
		char hostaddr[20];

		GetWindowText(GetDlgItem(hwndDlg, IDC_EDIT1), hostaddr, 20);
		portNo = GetDlgItemInt(hwndDlg, IDC_EDIT2, &retVal, FALSE);
		numPackets = GetDlgItemInt(hwndDlg, IDC_EDIT3, &retVal, FALSE);
		packetSize = GetDlgItemInt(hwndDlg, IDC_EDIT4, &retVal, FALSE);

		if(sendfile == TRUE) {
			BufferFile(NULL);
		}

 		TCP_Client(hostaddr, portNo);
}


DWORD WINAPI tcp_thrd(LPVOID param) {
	if(sendfile == FALSE) {
		for(int i = 0; i < numPackets-1; ++i) {
			SendMessage(hwnd, WM_TCPSOCKET, TCPsocket, FD_WRITE);
			Sleep(5);
		}
	} else {
		for(int i = 0; i < numPackets; ++i) {
			SendMessage(hwnd, WM_TCPSOCKET, TCPsocket, FD_WRITE);
		}
	}
	return 1;
}