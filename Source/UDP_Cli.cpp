#include <WinSock2.h>
#include "resource.h"
#include "network.h"
#include "externs.h"
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: UDP_Client
--
-- DATE: Feb 19, 2011
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Duncan Donaldson
--
-- PROGRAMMER: Duncan Donaldson
--
-- INTERFACE: void UDP_Client(char* serv_addr, int PortNo, int NumPackets)
--							serv_addr	- the server address.
--							PortNo		- the port number to bind to.
--							NumPackets	- the number of packets to send.
--
-- RETURNS: void
--
-- NOTES:
-- Initializes a UDP client and sends FD_WRITE messages for the number of packets to send.
----------------------------------------------------------------------------------------------------------------------*/
void UDP_Client(char* serv_addr, int PortNo) {

	SOCKET sock;
	HANDLE hThread;
	DWORD tid;

	WinsockInit();
	UDPSocket_Init(&sock);

	ZeroMemory(&udp_serv, sizeof(struct sockaddr_in));
	udp_serv.sin_family = AF_INET;
	udp_serv.sin_port = htons(PortNo);

	if ((udp_host = gethostbyname(serv_addr)) == NULL) {
		WSAError(SOCK_ERROR);
	}

	memmove((char *)&udp_serv.sin_addr, udp_host -> h_addr, udp_host -> h_length);

	WSAAsyncSelect(sock, hwnd, WM_UDPSOCKET, FD_WRITE|FD_CLOSE);
	UDPsocket = sock;

	if((hThread = CreateThread(NULL, 0, udp_thrd, NULL, NULL, &tid)) == 0) {
		WSAError(THREAD_ERROR);
	}
}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: Call_UDP_Cl
--
-- DATE: Feb 19, 2011
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Duncan Donaldson
--
-- PROGRAMMER: Duncan Donaldson
--
-- INTERFACE: void Call_UDP_Cl(HWND hwndDlg)
--							hwndDlg - handle to the dialog window to get initialization params from.
--
-- RETURNS: void
--
-- NOTES:
-- gets UDP client initialization parameters, and calls the UDP_Client function with said parameters.
----------------------------------------------------------------------------------------------------------------------*/
void Call_UDP_Cl(HWND hwndDlg) {

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

		UDP_Client(hostaddr, portNo);
}

DWORD WINAPI udp_thrd(LPVOID param) {
	if(sendfile == FALSE) {
		for(int i = 0; i < numPackets-1; ++i) {
			SendMessage(hwnd, WM_UDPSOCKET, UDPsocket, FD_WRITE);
			Sleep(5);
		}
	} else {
		for(int i = 0; i < numPackets; ++i) {
			SendMessage(hwnd, WM_UDPSOCKET, UDPsocket, FD_WRITE);
		}
	}
	return 1;
}