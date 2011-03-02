/*------------------------------------------------------------------------------------
-- SOURCE FILE:		main.cpp - A UDP / TCP client server application
--
-- PROGRAM:			Epic Client/Server
--
-- FUNCTIONS:		***************BUFFERING***************
--					void bufferData(char* data, int size)
--					DWORD WINAPI WriteBufToFile(LPVOID fpath)
--					int bufferData(char* buf, int size)
--					***************CALLBACKS***************
--					LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM)
--					BOOL CALLBACK Serv_DlgProc(HWND, UINT, WPARAM, LPARAM)
--					BOOL CALLBACK Cl_DlgProc(HWND, UINT, WPARAM, LPARAM)
--					***************NETWORK*****************
--					***************************************
--					void WinsockInit()
--					void WinsockCleanup(SOCKET socket)
--					void Add_Sock(SOCKET s)
--					PSOCK Find_Sock(SOCKET s)
--					void Remove_Sock(SOCKET s)
--					void WSAError(int error)
--					***************TCP*********************
--					void TCPSocket_Init(SOCKET* socket)
--					void TCPSocket_Listen(SOCKET* socket, int PortNo)
--					void TCPSocket_Connect(SOCKET* socket, int PortNo, char* serv_addr)
--					void TCP_Send(SOCKET s, int PacketSize, char* data)
--					void TCP_Client(char* serv_addr, int PortNo, int NumPackets)
--					void Call_TCP_Cl(HWND hwndDlg)
--					void TCP_Server(HWND hwnd, int PortNo)
--					***************UDP*********************
--					void UDPSocket_Init(SOCKET* socket)
--					void UDPSocket_Bind(SOCKET* socket, int PortNo)
--					void UDP_Send(SOCKET s, struct sockaddr* sa, int PacketSize, char* data)
--					void UDP_Client(char* serv_addr, int PortNo, int NumPackets)
--					void Call_UDP_Cl(HWND hwndDlg)
--					void UDP_Server(HWND hwnd, int PortNo)
--					***************FILES*******************
--					void GetFileName(HWND hwnd, LPSTR FileName, LPSTR FileTitle)
--					DWORD FileWrite(HANDLE file, char* data, DWORD size)
--					DWORD WINAPI BufferFile(LPVOID pSize)
--					***************STATS*******************
--					void Serv_Stats()
--					void Cli_Stats()
--					int TransferRate(DWORD TotalTime, int NumPackets, int PacketSize)
--					int avgPacketTransferRate(DWORD TotalTime, int NumPackets)
--					***************************************
--
-- DATE:			Feb 23, 2010
--
-- REVISIONS:		none
--
-- DESIGNER:		Duncan Donaldson
--
-- PROGRAMMER:		Duncan Donaldson
--
-- NOTES:
-- This application is a TCP/UDP client / server application. It is capable of transferring
-- files and sending test data, by either TCP or UDP. In addition to this it gathers
-- transmission statistics and prints them out in a user-friendly format.
------------------------------------------------------------------------------------------*/
#include <WinSock2.h>
#include"callbacks.h"
#include"resource.h"
#include "buffer.h"
#include "externs.h"
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{
    static TCHAR szAppName[] = TEXT ("ClientServ") ;
    MSG          msg ;
    WNDCLASS     wndclass ;

    wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
    wndclass.lpfnWndProc   = WndProc ;
    wndclass.cbClsExtra    = 0 ;
    wndclass.cbWndExtra    = 0 ;
    wndclass.hInstance     = hInstance ;
    wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
    wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
    wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
    wndclass.lpszMenuName  = MAKEINTRESOURCE(IDR_MENU1) ;
    wndclass.lpszClassName = szAppName ;

    if (!RegisterClass (&wndclass))
    {
         MessageBox (NULL, TEXT ("FaiL!"), 
                      szAppName, MB_ICONERROR) ;
         return 0 ;
    }
     
    hwnd = CreateWindow (szAppName,
                          TEXT ("Epic Client/Server"),
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          NULL,
                          NULL,
                          hInstance,
                          NULL) ;
     
    ShowWindow (hwnd, iCmdShow) ;
    UpdateWindow (hwnd) ;

	InitializeCriticalSection(&critsect);

    while (GetMessage (&msg, NULL, 0, 0)) {
		if(!IsDialogMessage(hwndDlg, &msg)) {
			TranslateMessage (&msg) ;
			DispatchMessage (&msg) ;
		}
    }
	return msg.wParam ;
}
