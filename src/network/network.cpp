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
void socket::TCPSocket_Init() {
    int sizebuf = BUFSIZE;

    if ((sock_ = WSASocket(AF_INET, SOCK_STREAM,0, NULL, 0, WSA_FLAG_OVERLAPPED)) == SOCKET_ERROR) {
        WSAError(SOCK_ERROR);
    }
    setsockopt(sock_, SOL_SOCKET, SO_RCVBUF, (char*)&sizebuf, sizeof(int));
    setsockopt(sock_, SOL_SOCKET, SO_SNDBUF, (char*)&sizebuf, sizeof(int));
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
void socket::TCPSocket_Bind() {

    ZeroMemory(&addr_, sizeof(struct sockaddr_in)); 
    addr_.sin_family = AF_INET;
    addr_.sin_port = htons(TCPPORT); 
    addr_.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock_, (struct sockaddr *)&addr_, sizeof(addr_)) == -1) {
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
void socket::TCPSocket_Listen() {

    if(listen(sock_, 5)) {
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
void socket::TCPSocket_Connect(char* servAddr_) {

    struct hostent *host;

    ZeroMemory(&addr_, sizeof(struct sockaddr_in));
    addr_.sin_family = AF_INET;
    addr_.sin_port = htons(TCPPORT);

    if ((host = gethostbyname(servAddr_)) == NULL) {
        WSAError(SOCK_ERROR);
    }

    memmove((char *)&addr_.sin_addr, host -> h_addr, host -> h_length);

    if (connect (sock_, (struct sockaddr *)&addr_, sizeof(addr_)) == SOCKET_ERROR) {
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
void socket::UDPSocket_Init() {
    int sizebuf = BUFSIZE;

    if ((sock_ = WSASocket(PF_INET, SOCK_DGRAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED)) == SOCKET_ERROR) {
        WSAError(SOCK_ERROR);
    }
    setsockopt(sock_, SOL_SOCKET, SO_RCVBUF, (char*)&sizebuf, sizeof(int));
    setsockopt(sock_, SOL_SOCKET, SO_SNDBUF, (char*)&sizebuf, sizeof(int));
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
void socket::UDPSocket_Bind_Multicast() {

    struct sockaddr_in serv;
    struct ip_mreq mc_addr;
    int ttl = MULTICAST_TTL;
    BOOL loopback = FALSE;
    
    ZeroMemory(&addr_, sizeof(struct sockaddr_in)); 
    addr_.sin_family = AF_INET;
    addr_.sin_port = htons(UDPPORT); 
    addr_.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock_, (struct sockaddr *)&addr_, sizeof(addr_)) == SOCKET_ERROR) {
        WSAError(SOCK_ERROR);
    }
    
    mc_addr.imr_multiaddr.s_addr = inet_addr(MULTICAST_ADDR);
    mc_addr.imr_interface.s_addr = INADDR_ANY;
    
    if(setsockopt(sock_, IPPROTO_IP, IP_ADD_MEMBERSHIP, 
                            (char *)&mc_addr, sizeof(mc_addr)) == SOCKET_ERROR) {
        WSAError(SOCK_ERROR);                    
    }
    
    if(setsockopt(sock_, IPPROTO_IP, IP_MULTICAST_TTL, 
                            (char *)&ttl, sizeof(ttl)) == SOCKET_ERROR) {
        WSAError(SOCK_ERROR);                    
    }
    
    if(setsockopt(sock_, IPPROTO_IP, IP_MULTICAST_LOOP, 
                            (char *)&loopback, sizeof(BOOL)) == SOCKET_ERROR) {
        WSAError(SOCK_ERROR);                    
    }
}
void socket::TCPSend(char* buf) {
    
}
void socket::UDPSend_Multicast(char* buf) {
    addr_.sin_family = AF_INET;
    addr_.sin_addr.s_addr = inet_addr(MULTICAST_ADDR);
    addr_.sin_port = htons(UDPPORT);
    //WSASendTo, no completion routine, but overlapped struct.
}
void socket::TCPRecv(char* buf) {

}
void socket::UDPRecv_Multicast(char* buf) {
    //WSARecv with completion routine.
}
void socket::CALLBACK UDPCompRoutine(DWORD error, DWORD cbTransferred,
                        LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags) {
                        
}
void socket::CALLBACK TCPCompRoutine(DWORD error, DWORD transferred,
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
