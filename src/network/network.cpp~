#include <crtdbg.h>
#include <WinSock2.h>
#include "network.h"
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: WinsockInit
--
-- DATE: Feb 19, 2011
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Duncan Donaldson
--
-- PROGRAMMER: Duncan Donaldson
--
-- INTERFACE: void WinsockInit()
--
-- RETURNS: void
--
-- NOTES:
-- Initializes a Winsock session.
----------------------------------------------------------------------------------------------------------------------*/
void WinsockInit() {

	WORD version;
	WSADATA wsd;

	version = MAKEWORD(2,2);
	if(WSAStartup(version, &wsd) != 0) {
		WSAError(SOCK_ERROR);
	}

}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: WinsockCleanup
--
-- DATE: Feb 19, 2011
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Duncan Donaldson
--
-- PROGRAMMER: Duncan Donaldson
--
-- INTERFACE: void WinsockCleanup(socket)
--
-- RETURNS: void
--
-- NOTES:
-- cleans up a winsock session.
----------------------------------------------------------------------------------------------------------------------*/
void WinsockCleanup() {

	WSACleanup();

}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: TCPSocket_Init
--
-- DATE: Feb 19, 2011
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Duncan Donaldson
--
-- PROGRAMMER: Duncan Donaldson
--
-- INTERFACE: void TCPSocket_Init(SOCKET* socket)
--				socket - pointer to the socket to be initialized
--
-- RETURNS: void
--
-- NOTES:
-- initializes a TCP socket.
----------------------------------------------------------------------------------------------------------------------*/
void TCPSocket_Init(SOCKET* sock) {
	int sizebuf = 65000000;

	if (((*sock) = socket(AF_INET, SOCK_STREAM,0)) == SOCKET_ERROR) {
		WSAError(SOCK_ERROR);
	}
	setsockopt(*sock, SOL_SOCKET, SO_RCVBUF, (char*)&sizebuf, sizeof(int));
	setsockopt(*sock, SOL_SOCKET, SO_SNDBUF, (char*)&sizebuf, sizeof(int));
}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: TCPSocket_Listen
--
-- DATE: Feb 19, 2011
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Duncan Donaldson
--
-- PROGRAMMER: Duncan Donaldson
--
-- INTERFACE: void TCPSocket_Listen(SOCKET* socket, int PortNo)
--				socket - pointer to the socket to be initialized
--				PortNo - the port number to bind to.
--
-- RETURNS: void
--
-- NOTES:
-- binds a TCP socket to a port, and sets it to the listen state.
----------------------------------------------------------------------------------------------------------------------*/
void TCPSocket_Bind(SOCKET* sock, int PortNo) {

	struct	sockaddr_in serv;

	ZeroMemory(&serv, sizeof(struct sockaddr_in)); 
	serv.sin_family = AF_INET;
	serv.sin_port = htons(PortNo); 
	serv.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(*sock, (struct sockaddr *)&serv, sizeof(serv)) == -1) {
		WSAError(SOCK_ERROR);
	}

}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: TCPSocket_Listen
--
-- DATE: Feb 19, 2011
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Duncan Donaldson
--
-- PROGRAMMER: Duncan Donaldson
--
-- INTERFACE: void TCPSocket_Listen(SOCKET* socket, int PortNo)
--				socket - pointer to the socket to be initialized
--				PortNo - the port number to bind to.
--
-- RETURNS: void
--
-- NOTES:
-- Sets a TCP socket to the listen state.
----------------------------------------------------------------------------------------------------------------------*/
void TCPSocket_Listen(SOCKET* sock) {
    if(listen(*sock, 5)) {
        WSAError(SOCK_ERROR);
    }
}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: TCPSocket_Connect
--
-- DATE: Feb 19, 2011
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Duncan Donaldson
--
-- PROGRAMMER: Duncan Donaldson
--
-- INTERFACE: void TCPSocket_Connect(SOCKET* socket, int PortNo, char* serv_addr)
--				socket		- pointer to the socket to be initialized
--				PortNo		- the port number to connect on.
--				serv_addr	- the server address to connect to.
--
-- RETURNS: void
--
-- NOTES:
-- attempts to connect a TCP socket to a server.
----------------------------------------------------------------------------------------------------------------------*/
void TCPSocket_Connect(SOCKET* sock, int PortNo, char* serv_addr) {

	struct sockaddr_in serv;
	struct hostent	*host;

	ZeroMemory(&serv, sizeof(struct sockaddr_in));
	serv.sin_family = AF_INET;
	serv.sin_port = htons(PortNo);

	if ((host = gethostbyname(serv_addr)) == NULL) {
		WSAError(SOCK_ERROR);
	}

	memmove((char *)&serv.sin_addr, host -> h_addr, host -> h_length);

	if (connect (*sock, (struct sockaddr *)&serv, sizeof(serv)) == SOCKET_ERROR) {
		WSAError(SOCK_ERROR);
	}
}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: UDPSocket_Init
--
-- DATE: Feb 19, 2011
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Duncan Donaldson
--
-- PROGRAMMER: Duncan Donaldson
--
-- INTERFACE: void UDPSocket_Init(SOCKET* socket)
--				socket - pointer to the socket to be initialized
--
-- RETURNS: void
--
-- NOTES:
-- initializes a UDP socket.
----------------------------------------------------------------------------------------------------------------------*/
void UDPSocket_Init(SOCKET* sock) {
	int sizebuf = 65000000;

	if (((*sock) = socket(PF_INET, SOCK_DGRAM, 0)) == SOCKET_ERROR) {
		WSAError(SOCK_ERROR);
	}
	setsockopt(*sock, SOL_SOCKET, SO_RCVBUF, (char*)&sizebuf, sizeof(int));
	setsockopt(*sock, SOL_SOCKET, SO_SNDBUF, (char*)&sizebuf, sizeof(int));
}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: UDPSocket_Bind
--
-- DATE: Feb 19, 2011
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Duncan Donaldson
--
-- PROGRAMMER: Duncan Donaldson
--
-- INTERFACE: void UDPSocket_Bind(SOCKET* socket, int PortNo)
--				socket - pointer to the socket to be initialized
--				PortNo - the port to bind to.
--
-- RETURNS: void
--
-- NOTES:
-- binds a UDP ocket to a port.
----------------------------------------------------------------------------------------------------------------------*/
void UDPSocket_Bind(SOCKET* sock, int PortNo) {

		struct	sockaddr_in serv;

		ZeroMemory(&serv, sizeof(struct sockaddr_in)); 
		serv.sin_family = AF_INET;
		serv.sin_port = htons(PortNo); 
		serv.sin_addr.s_addr = htonl(INADDR_ANY);

		if (bind(*sock, (struct sockaddr *)&serv, sizeof(serv)) == SOCKET_ERROR) {
			WSAError(SOCK_ERROR);
		}

}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: WSAError
--
-- DATE: Feb 19, 2011
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Duncan Donaldson
--
-- PROGRAMMER: Duncan Donaldson
--
-- INTERFACE: void WSAError(int error)
--				error - the error code.
--
-- RETURNS: void.
--
-- NOTES:
-- handles an error, by displaying a helpful message box and exiting.
----------------------------------------------------------------------------------------------------------------------*/
void WSAError(int error) {
	MessageBox(NULL, "SOMETHING WENT WRONG!", "FATAL ERROR", MB_OK);
	exit(error);
}
