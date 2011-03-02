#include <crtdbg.h>
#include <WinSock2.h>
#include "network.h"
#include "externs.h"
#include "resource.h"
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
-- INTERFACE: void WinsockCleanup(SOCKET socket)
--				socket - the socket to be closed
--
-- RETURNS: void
--
-- NOTES:
-- cleans up a winsock session and destroys a socket.
----------------------------------------------------------------------------------------------------------------------*/
void WinsockCleanup(SOCKET socket) {

	closesocket(socket);
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
void TCPSocket_Listen(SOCKET* sock, int PortNo) {

	struct	sockaddr_in serv;

	ZeroMemory(&serv, sizeof(struct sockaddr_in)); 
	serv.sin_family = AF_INET;
	serv.sin_port = htons(PortNo); 
	serv.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(*sock, (struct sockaddr *)&serv, sizeof(serv)) == -1) {
		WSAError(SOCK_ERROR);
	}

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
-- FUNCTION: Add_Sock
--
-- DATE: Feb 19, 2011
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Duncan Donaldson
--
-- PROGRAMMER: Duncan Donaldson
--
-- INTERFACE: void Add_Sock(SOCKET s)
--				s - socket to be addded to the new SOCK structure.
--
-- RETURNS: void
--
-- NOTES:
-- Allocates a new SOCK structure and adds it to the currently existing list of sockets.
----------------------------------------------------------------------------------------------------------------------*/
void Add_Sock(SOCKET s) {

	PSOCK si;

    if ((si = (PSOCK) GlobalAlloc(GPTR, sizeof(SOCK))) == NULL) {
      WSAError(ALLOC_ERROR);
    }

    si -> sock = s;
    si -> NumRcv = 0;

    si -> next = sockets;
    sockets = si;

}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: Find_Sock
--
-- DATE: Feb 19, 2011
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Duncan Donaldson
--
-- PROGRAMMER: Duncan Donaldson
--
-- INTERFACE: PSOCK Find_Sock(SOCKET s)
--				s - the socket to be found.
--
-- RETURNS: the PSOCK found.
--
-- NOTES:
-- finds a PSOCK structure based on a given socket.
----------------------------------------------------------------------------------------------------------------------*/
PSOCK Find_Sock(SOCKET s) {

    PSOCK tmp = sockets;

    while(tmp != NULL) {
       if (tmp -> sock == s) {
          return tmp;
	   }
       tmp = tmp -> next;
    }
    return 0;

}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: Remove_Sock
--
-- DATE: Feb 19, 2011
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Duncan Donaldson
--
-- PROGRAMMER: Duncan Donaldson
--
-- INTERFACE: void Remove_Sock(SOCKET s)
--				s - the socket to be reomved.
--
-- RETURNS: the PSOCK found.
--
-- NOTES:
-- removes a PSOCK structure based on a given socket.
----------------------------------------------------------------------------------------------------------------------*/
void Remove_Sock(SOCKET s) {

	PSOCK cur = sockets;
    PSOCK prev = NULL;

    while(cur != NULL) {
       if (cur -> sock == s) {
          if (prev) {
             prev -> next = cur -> next;
		  } else {
             sockets = cur -> next;
		  }
          closesocket(cur -> sock);
          GlobalFree(cur);
          return;
	   }
       prev = cur;
       cur = cur -> next;
    }

}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: TCP_Send
--
-- DATE: Feb 19, 2011
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Duncan Donaldson
--
-- PROGRAMMER: Duncan Donaldson
--
-- INTERFACE: void TCP_Send(SOCKET s, int PacketSize, char* data)
--				s			- socket to write to.
--				PacketSize	- the size of the data to be written.
--				data		- the data to send.
--
-- RETURNS: void.
--
-- NOTES:
-- writes some data to a TCP socket, if data is 0, writes a test packet.
----------------------------------------------------------------------------------------------------------------------*/
int TCP_Send(SOCKET s, int PacketSize, char* data) {

	char* sndBuf = NULL;
	WSABUF buf;
	DWORD bSent;
	int err;

	if(data == NULL) {
		sndBuf = (char*)malloc(PacketSize);
		if(sndBuf == NULL) {
			WSAError(ALLOC_ERROR);
		}
		memset((char *)sndBuf, 'A', PacketSize);
		buf.buf = sndBuf;
	} else {
		buf.buf = data;
	}

	buf.len = PacketSize;

	if(WSASend (s, &buf, 1, &bSent, 0, NULL, NULL) == SOCKET_ERROR) {
		if ((err = WSAGetLastError()) != WSAEWOULDBLOCK) {
			WSAError(WR_ERROR);
		}
		if(sndBuf != NULL) {
			free(sndBuf);
		}
		return 0;
	} else if(bSent != 0) {
		stats.TOTALDATA+=bSent;
		stats.TOTALTCPPACKETS++;
		PostMessage(hwndDlg, WM_STATS, NULL, NULL);
	}

	if(sndBuf != NULL) {
		free(sndBuf);
	}
	return 1;
}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: UDP_Send
--
-- DATE: Feb 19, 2011
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Duncan Donaldson
--
-- PROGRAMMER: Duncan Donaldson
--
-- INTERFACE: void UDP_Send(SOCKET s, struct sockaddr* sa, int PacketSize, char* data)
--				s			- socket to write to.
-				sa			- the address to send the data to.
--				PacketSize	- the size of the data to be written.
--				data		- the data to send.
--
-- RETURNS: void.
--
-- NOTES:
-- writes some data to a UDP socket of a specified address, if data is 0, writes a test packet.
----------------------------------------------------------------------------------------------------------------------*/
int UDP_Send(SOCKET s, struct sockaddr* sa, int PacketSize, char* data) {

	WSABUF buf;
	char* sndBuf = NULL;
	DWORD bSent;
	int err;


	if(data == NULL) {
		sndBuf = (char*)malloc(PacketSize);
		if(sndBuf == NULL) {
			WSAError(ALLOC_ERROR);
		}
		memset((char *)sndBuf, 'A', PacketSize);
		buf.buf = sndBuf;
	} else {
		buf.buf = data;
	}


	buf.len = PacketSize;
	if(WSASendTo (s, &buf, 1, &bSent, 0, sa, sizeof(struct sockaddr), NULL, NULL) == SOCKET_ERROR) {
		if ((err = WSAGetLastError()) != WSAEWOULDBLOCK) {
			WSAError(err);
		}
		if(sndBuf != NULL) {
			free(sndBuf);
		}
		return 0;
	} else if(bSent != 0){
		stats.TOTALDATA+=bSent;
		stats.TOTALUDPPACKETS++;
		PostMessage(hwndDlg, WM_STATS, NULL, NULL);
	}

	if(sndBuf != NULL) {
		free(sndBuf);
	}
	return 1;
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
