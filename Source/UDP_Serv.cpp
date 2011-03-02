#include "network.h"
#include "callbacks.h"
#include "externs.h"
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: UDP_Server
--
-- DATE: Feb 19, 2011
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Duncan Donaldson
--
-- PROGRAMMER: Duncan Donaldson
--
-- INTERFACE: void UDP_Server(HWND hwnd, int PortNo)
--							hwnd		- the window to get init params from.
--							PortNo		- the port number to bind to.
--
-- RETURNS: void
--
-- NOTES:
-- Initializes a UDP socket and binds it to a selected port, also sets the WSAASYNCSELECT messages.
----------------------------------------------------------------------------------------------------------------------*/
void UDP_Server(HWND hwnd, int PortNo) {

	SOCKET sock;

	WinsockInit();
	UDPSocket_Init(&sock);
	UDPSocket_Bind(&sock, PortNo);
	WSAAsyncSelect(sock, hwnd, WM_UDPSOCKET, FD_READ|FD_CLOSE);
	UDPsocket = sock;

}

