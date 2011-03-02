#include <crtdbg.h>
#include <WinSock2.h>
#include <stdio.h>
#include "callbacks.h"
#include "helpers.h"
#include "resource.h"
#include "network.h"
#include "buffer.h"
#include "externs.h"
#include "stats.h"
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: WndProc
--
-- DATE: Feb 19, 2011
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Duncan Donaldson
--
-- PROGRAMMER: Duncan Donaldson
--
-- INTERFACE: LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
--							- hwnd	   - the handle to the program's main window.
--							- message  - the message to be handled
--							- wParam   - the wParam of the message
--							- lParam   - the lParam of the message
--
--
-- RETURNS: LRESULT - arbitrary return value.
--
-- NOTES:
-- handles messages to the program's mainwindow, this contains a large portion of the network read/write code.
----------------------------------------------------------------------------------------------------------------------*/
LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {

    HDC         hdc ;
    PAINTSTRUCT ps ;
	SOCKET aSock;
	WSABUF dbuf;
	char* buf = 0;
	char rdbuf[BUFSIZE];
	PSOCK curSock;
	DWORD flags = 0, numRead = 0;

    switch (message) {

		case WM_UDPSOCKET:

			switch(WSAGETSELECTEVENT(lParam)) {
				//UDP READ
				case FD_READ:

					dbuf.buf = rdbuf;
					dbuf.len = BUFSIZE;

					EnterCriticalSection(&critsect);
					if(stats.TOTALUDPPACKETS == 1) {
						stats.START_TIME = GetTickCount();
					}
					if(WSARecv(wParam, &dbuf, 1, &numRead, &flags, NULL, NULL) == SOCKET_ERROR) {
						if(WSAGetLastError() != WSAEWOULDBLOCK) {
							WSAError(RD_ERROR);
						}
					} else if(numRead != 0) {
						FileWrite(file, dbuf.buf, numRead); 
						stats.TOTALUDPPACKETS++;
						stats.TOTALDATA+=numRead;
						PostMessage(hwndDlg, WM_STATS, NULL, NULL);
					}
					LeaveCriticalSection(&critsect);
					break;
				//UDP WRITE
				case FD_WRITE:

					if(sendfile == TRUE) {

						buf = (char*)malloc(packetSize);
						if(buf == NULL) {
							WSAError(ALLOC_ERROR);
						}
						EnterCriticalSection(&critsect);
						if(popTail(buf, packetSize) != 0) {
							UDP_Send(wParam, (struct sockaddr*)&udp_serv, packetSize, buf);
						}
						free(buf);
						LeaveCriticalSection(&critsect);

					} else {
						UDP_Send(wParam, (struct sockaddr*)&udp_serv, packetSize, 0);
					}
					break;

				case FD_CLOSE:
					closesocket(wParam);
					WinsockCleanup(wParam);
					break;

			}	
			return 0;

		case WM_TCPSOCKET:

			if(WSAGETSELECTERROR(lParam)) {
				Remove_Sock(wParam);
				break;
			}

			switch(WSAGETSELECTEVENT(lParam)) {

				//TCP ACCEPT
				case FD_ACCEPT:

				   if ((aSock = accept(wParam, NULL, NULL)) == INVALID_SOCKET) {
					  WSAError(SOCKET_ERROR);
				   }

				   Add_Sock(aSock);
				   WSAAsyncSelect(aSock, hwnd, WM_TCPSOCKET, FD_READ|FD_CLOSE);

				   break;

				//TCP WRITE
				case FD_WRITE:

					if(sendfile == TRUE) {

						buf = (char*)malloc(packetSize);
						if(buf == NULL) {
							WSAError(ALLOC_ERROR);
						}
						EnterCriticalSection(&critsect);
						if(popTail(buf, packetSize) != 0) {
							TCP_Send(wParam, packetSize, buf);
						}
						LeaveCriticalSection(&critsect);
						free(buf);

					} else {
						TCP_Send(wParam, packetSize, 0);
					}
					break;
				
				//TCP READ
				case FD_READ:

					curSock = Find_Sock(wParam);
					if(curSock == 0) {
						return 0;
					}

					curSock -> dbuf.buf = curSock -> buf;
					curSock -> dbuf.len = BUFSIZE;
					flags = 0;
					if(stats.TOTALTCPPACKETS == 1) {
						stats.START_TIME = GetTickCount();
					}
					EnterCriticalSection(&critsect);
					if(WSARecv(curSock -> sock, &(curSock -> dbuf), 1, &numRead
							, &flags, NULL, NULL) == SOCKET_ERROR) {
						if (WSAGetLastError() != WSAEWOULDBLOCK) {
							WSAError(RD_ERROR);
						}
					} else if(numRead != 0) {
						FileWrite(file, (curSock->dbuf).buf, numRead); 
						stats.TOTALTCPPACKETS++;
						stats.TOTALDATA+=numRead;
						PostMessage(hwndDlg, WM_STATS, NULL, NULL);
					}
					LeaveCriticalSection(&critsect);
				    break;

				//TCP CLOSE
				case FD_CLOSE:
					Remove_Sock(wParam);
					WinsockCleanup(wParam);
					break;
			}
			return 0;

		case WM_COMMAND:

			switch(LOWORD(wParam)) {
				case ID_MODE_CLIENT:

					hwndDlg = CreateDialog(GetModuleHandle(0), MAKEINTRESOURCE(IDD_DIALOG1), hwnd, Cl_DlgProc);
					ShowWindow(hwndDlg, SW_NORMAL);
					ShowWindow(hwnd, SW_HIDE);
					break;
				case ID_MODE_SERVER:
					hwndDlg = CreateDialog(GetModuleHandle(0), MAKEINTRESOURCE(IDD_DIALOG2), hwnd, Serv_DlgProc);
					ShowWindow(hwndDlg, SW_NORMAL);
					ShowWindow(hwnd, SW_HIDE);
					break;
				case ID_MENU_QUIT40012:
					PostQuitMessage(0);
					break;
			}
			return 0;

		case WM_PAINT:

			hdc = BeginPaint (hwnd, &ps) ; 
			EndPaint (hwnd, &ps) ;
			return 0 ;

		case WM_DESTROY:

			WSACleanup();
			PostQuitMessage (0) ;
			return 0 ;

    }
    return DefWindowProc (hwnd, message, wParam, lParam) ;
}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: Serv_DlgProc
--
-- DATE: Feb 19, 2011
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Duncan Donaldson
--
-- PROGRAMMER: Duncan Donaldson
--
-- INTERFACE: BOOL CALLBACK Serv_DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
--							- hwnd	   - the handle to the program's main window.
--							- message  - the message to be handled
--							- wParam   - the wParam of the message
--							- lParam   - the lParam of the message
--
--
-- RETURNS: BOOL - true if message handled, false otherwise.
--
-- NOTES:
-- handles messages to the server dialog proc.
----------------------------------------------------------------------------------------------------------------------*/
BOOL CALLBACK Serv_DlgProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam) {

	int proto, portNo;
	char port[6];

	switch(message) {
		case WM_INITDIALOG:

			CheckDlgButton(hwndDlg, IDC_RADIO3, 1); 
			return TRUE;

		case WM_STATS:

			Serv_Stats();
			return TRUE;

		case WM_COMMAND:

			switch(LOWORD(wParam)) {
				//disconnect
				case IDC_BUTTON1:

					if(stats.TYPE == STATTCP) {
						SendMessage(hwnd, WM_TCPSOCKET, TCPsocket, FD_CLOSE); 
					} else if(stats.TYPE == STATUDP) {
						SendMessage(hwnd, WM_UDPSOCKET, UDPsocket, FD_CLOSE); 
					}
					CloseHandle(file);
					ResetGlobals();
					stats.STATE = STATDISCONNECT;
					PostMessage(hwndDlg, WM_STATS, NULL, NULL);
					break;

				case IDOK:

					//open file
					if(!GetFileName(hwndDlg, FilePath, FileName)) {
						return 0;
					}
					if(!(file = CreateFile(FilePath, FILE_APPEND_DATA, FILE_SHARE_WRITE,
							NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL))) {
						WSAError(FILE_ERROR);
					}
					//gather stats and determine protocol
					proto = SendMessage(GetDlgItem(hwndDlg, IDC_RADIO3), BM_GETCHECK, 0, 1);
					stats.STATE = STATCONNECT;
					PostMessage(hwndDlg, WM_STATS, NULL, NULL);

					switch(proto) {
						//called with TCP
						case BST_CHECKED:

							GetWindowText(GetDlgItem(hwndDlg, IDC_EDIT5), port, 6);
							portNo = atoi(port);
							stats.TYPE = STATTCP;
							TCP_Server(hwnd, portNo);
							break;

						//called with UDP
						case BST_UNCHECKED:

							GetWindowText(GetDlgItem(hwndDlg, IDC_EDIT5), port, 6);
							portNo = atoi(port);
							stats.TYPE = STATUDP;
							UDP_Server(hwnd, portNo);
							break;
					}
					break;

				case IDCANCEL:
					SendMessage(hwnd, WM_DESTROY, NULL, NULL);
					break;
			}
			return TRUE;

		default:
			break;

	}
	return FALSE;
}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: Cl_DlgProc
--
-- DATE: Feb 19, 2011
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Duncan Donaldson
--
-- PROGRAMMER: Duncan Donaldson
--
-- INTERFACE: BOOL CALLBACK Cl_DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
--							- hwnd	   - the handle to the program's main window.
--							- message  - the message to be handled
--							- wParam   - the wParam of the message
--							- lParam   - the lParam of the message
--
--
-- RETURNS: BOOL - true if message handled, false otherwise.
--
-- NOTES:
-- handles messages to the client dialog proc.
----------------------------------------------------------------------------------------------------------------------*/
BOOL CALLBACK Cl_DlgProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam) {

	int proto;

	switch(message) {

		case WM_INITDIALOG:

			CheckDlgButton(hwndDlg, IDC_RADIO1, 1); 
			return TRUE;

		case WM_STATS:

			Cli_Stats();
			return TRUE;

		case WM_COMMAND:

			switch(LOWORD(wParam)) {
				//DISCONNECT
				case IDC_BUTTON1:

					if(stats.TYPE == STATTCP) {
						SendMessage(hwnd, WM_TCPSOCKET, TCPsocket, FD_CLOSE); 
					} else if(stats.TYPE == STATUDP) {
						SendMessage(hwnd, WM_UDPSOCKET, UDPsocket, FD_CLOSE); 
					}
					ResetGlobals();
					CloseHandle(file);
					stats.STATE = STATDISCONNECT;
					PostMessage(hwndDlg, WM_STATS, NULL, NULL);
					break;
				//send file
				case IDC_BUTTON2:

					if(!GetFileName(hwndDlg, FilePath, FileName)) {
						return TRUE;
					} else {

						sendfile = TRUE;

						proto = SendMessage(GetDlgItem(hwndDlg, IDC_RADIO1), BM_GETCHECK, 0, 1);
						if(proto == BST_CHECKED) {
							Call_TCP_Cl(hwndDlg);
							break;
						} else if((proto = SendMessage(GetDlgItem(hwndDlg, IDC_RADIO2), BM_GETCHECK, 0, 1)) == BST_CHECKED) {
							Call_UDP_Cl(hwndDlg);	
						}
					}
					break;

				//send test.
				case IDOK:

					stats.STATE = STATCONNECT;
					PostMessage(hwndDlg, WM_STATS, NULL, NULL);
					proto = SendMessage(GetDlgItem(hwndDlg, IDC_RADIO1), BM_GETCHECK, 0, 1);
					if(proto == BST_CHECKED) {
						Call_TCP_Cl(hwndDlg);
						break;
					} else if((proto = SendMessage(GetDlgItem(hwndDlg, IDC_RADIO2), BM_GETCHECK, 0, 1)) == BST_CHECKED) {
						Call_UDP_Cl(hwndDlg);			
					}
					break;

				case IDCANCEL:
					SendMessage(hwnd, WM_DESTROY, NULL, NULL);
					break;

			}
			return TRUE;

		default:
			break;

	}
	return FALSE;
}