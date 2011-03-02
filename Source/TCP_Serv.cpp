#include "resource.h"
#include "network.h"
#include "callbacks.h"
#include "externs.h"
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: TCP_Server
--
-- DATE: Feb 19, 2011
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Duncan Donaldson
--
-- PROGRAMMER: Duncan Donaldson
--
-- INTERFACE: void TCP_Server(HWND hwnd, int PortNo)
--							hwnd		- the window to get init params from.
--							PortNo		- the port number to bind to and listen on.
--
-- RETURNS: void
--
-- NOTES:
-- Initializes a TCP Server and puts it in the listen state.
----------------------------------------------------------------------------------------------------------------------*/
void TCP_Server(HWND hwnd, int PortNo) {

	SOCKET lSock;

	WinsockInit();
	TCPSocket_Init(&lSock);
	WSAAsyncSelect(lSock, hwnd, WM_TCPSOCKET, FD_ACCEPT|FD_CLOSE);
	TCPSocket_Listen(&lSock, PortNo);
	Add_Sock(lSock);
	TCPsocket = lSock;

}

