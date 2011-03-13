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
  int sizebuf = BUFSIZE;

  if (((*sock) = WSASocket(AF_INET, SOCK_STREAM,0, NULL, 0, WSA_FLAG_OVERLAPPED)) == SOCKET_ERROR) {
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
void TCPSocket_Bind(SOCKET* sock) {

  struct sockaddr_in serv;

  ZeroMemory(&serv, sizeof(struct sockaddr_in)); 
  serv.sin_family = AF_INET;
  serv.sin_port = htons(TCPPORT); 
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
void TCPSocket_Connect(SOCKET* sock, char* serv_addr) {

  struct sockaddr_in serv;
  struct hostent *host;

  ZeroMemory(&serv, sizeof(struct sockaddr_in));
  serv.sin_family = AF_INET;
  serv.sin_port = htons(TCPPORT);

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
  int sizebuf = BUFSIZE;

  if (((*sock) = WSASocket(PF_INET, SOCK_DGRAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED)) == SOCKET_ERROR) {
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
void UDPSocket_Bind_Multicast(SOCKET* sock) {

    struct sockaddr_in serv;
    struct ip_mreq mc_addr;
    int ttl = MULTICAST_TTL;
    BOOL loopback = FALSE;
    
    ZeroMemory(&serv, sizeof(struct sockaddr_in)); 
    serv.sin_family = AF_INET;
    serv.sin_port = htons(UDPPORT); 
    serv.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(*sock, (struct sockaddr *)&serv, sizeof(serv)) == SOCKET_ERROR) {
        WSAError(SOCK_ERROR);
    }
    
    mc_addr.imr_multiaddr.s_addr = inet_addr(MULTICAST_ADDR);
    mc_addr.imr_interface.s_addr = INADDR_ANY;
    
    if(setsockopt(hSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, 
                            (char *)&mc_addr, sizeof(mc_addr)) == SOCKET_ERROR) {
        WSAError(SOCK_ERROR);                    
    }
    
    if(setsockopt(hSocket, IPPROTO_IP, IP_MULTICAST_TTL, 
                            (char *)&ttl, sizeof(ttl)) == SOCKET_ERROR) {
        WSAError(SOCK_ERROR);                    
    }
    
    if(setsockopt(hSocket, IPPROTO_IP, IP_MULTICAST_LOOP, 
                            (char *)&loopback, sizeof(BOOL)) == SOCKET_ERROR) {
        WSAError(SOCK_ERROR);                    
    }
}
void TCPSend(SOCKET* sock, char* buf, int msgsize) {
    
}
void UDPSend_Multicast(SOCKET* sock, char* buf, int msgsize, struct sockaddr* addr) {
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(MULTICAST_ADDR);
    addr.sin_port = htons(UDPPORT);
    //WSASendTo, no completion routine, but overlapped struct.
}
void TCPRecv(SOCKET* sock, char* buf, int msgsize) {

}
void UDPRecv_Multicast(SOCKET* sock, char* buf, int msgsize) {
    //WSARecv with completion routine.
}
void CALLBACK UDPCompRoutine(DWORD error, DWORD cbTransferred,
                        LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags) {
                        
}
void CALLBACK TCPCompRoutine(DWORD error, DWORD cbTransferred,
                        LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags) {
                        
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
  exit(error);
}
