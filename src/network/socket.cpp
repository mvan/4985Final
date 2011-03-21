#include <winsock2.h>
#include <ws2tcpip.h>
#include "network.h"
#include "socket.h"
#include "errors.cpp"
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
void sock::TCPSocket_Init() {
    int sizebuf = BUFSIZE;

    if ((sock_ = WSASocket(AF_INET, SOCK_STREAM,0, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET) {
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
BOOL sock::TCPSocket_Bind(int portNo) {

    ZeroMemory(&addr_, sizeof(struct sockaddr_in)); 
    addr_.sin_family = AF_INET;
    addr_.sin_port = htons(portNo);
    addr_.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock_, (struct sockaddr *)&addr_, sizeof(addr_)) == -1) {
        return FALSE;
    }
    return TRUE;
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
void sock::TCPSocket_Listen() {

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
BOOL sock::TCPSocket_Connect(char* servAddr, int portNo) {

    struct hostent *host;

    ZeroMemory(&addr_, sizeof(struct sockaddr_in));
    addr_.sin_family = AF_INET;
    addr_.sin_port = htons(portNo);

    if ((host = gethostbyname(servAddr)) == NULL) {
        return FALSE;
    }

    memmove((char *)&addr_.sin_addr, host -> h_addr, host -> h_length);

    if (connect (sock_, (struct sockaddr *)&addr_, sizeof(addr_)) == SOCKET_ERROR) {
        return FALSE;
    }
    return TRUE;
}

sock sock::TCPSocket_Accept() {
    int addrsize;
    SOCKET s;
    if((s = accept(sock_, (struct sockaddr*)&addr_, &addrsize)) == INVALID_SOCKET) {
        WSAError(SOCK_ERROR);
    }
    return sock(s);
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
void sock::UDPSocket_Init() {
    int sizebuf = BUFSIZE;

    if ((sock_ = WSASocket(PF_INET, SOCK_DGRAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET) {
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
BOOL sock::UDPSocket_Bind_Multicast(int portNo) {

    struct ip_mreq mc_addr;
    int ttl = MULTICAST_TTL;
    BOOL loopback = FALSE;
    
    ZeroMemory(&addr_, sizeof(struct sockaddr_in)); 
    addr_.sin_family = AF_INET;
    addr_.sin_port = htons(portNo);
    addr_.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock_, (struct sockaddr *)&addr_, sizeof(addr_)) == SOCKET_ERROR) {
        return FALSE;
    }
    
    mc_addr.imr_multiaddr.s_addr = inet_addr(MULTICAST_ADDR);
    mc_addr.imr_interface.s_addr = INADDR_ANY;
    
    if(setsockopt(sock_, IPPROTO_IP, IP_ADD_MEMBERSHIP, 
                            (char *)&mc_addr, sizeof(mc_addr)) == SOCKET_ERROR) {
        return FALSE;
    }
    
    if(setsockopt(sock_, IPPROTO_IP, IP_MULTICAST_TTL, 
                            (char *)&ttl, sizeof(ttl)) == SOCKET_ERROR) {
        return FALSE;
    }
    
    if(setsockopt(sock_, IPPROTO_IP, IP_MULTICAST_LOOP, 
                            (char *)&loopback, sizeof(BOOL)) == SOCKET_ERROR) {
        return FALSE;
    }
    return TRUE;
}
int sock::TCPSend() {
    WSABUF buf;
    DWORD wait;

    buf.buf = packet_;
    buf.len = PACKETSIZE;

    WSASend(sock_, &buf, 1, NULL, 0, &ol_, TCPSendCompRoutine);
    wait = WSAWaitForMultipleEvents(1, &(ol_.hEvent), FALSE, INFINITE, TRUE);
    if(wait == WSA_WAIT_FAILED) {
        return 0;
    }
    return 1;
}
int sock::UDPSend_Multicast() {
    WSABUF buf;
    DWORD wait;
    buf.buf = packet_;
    buf.len = PACKETSIZE;
    addr_.sin_family = AF_INET;
    addr_.sin_addr.s_addr = inet_addr(MULTICAST_ADDR);
    addr_.sin_port = htons(UDPPORT);
    WSASendTo(this->sock_, &buf, 1, NULL, 0, (struct sockaddr*)&addr_,
                        sizeof(addr_), &(this->ol_), UDPSendCompRoutine);

    wait = WSAWaitForMultipleEvents(1, &(ol_.hEvent), FALSE, INFINITE, TRUE);
    WSAResetEvent(ol_.hEvent);
    if(wait == WSA_WAIT_FAILED) {
        return 0;
    }
    return 1;
}
int sock::TCPRecv() {
    return 0;
}
int sock::UDPRecv_Multicast() {
    DWORD flags = 0, wait;
    WSABUF buf;
    buf.buf = packet_;
    buf.len = PACKETSIZE;

    WSARecv(sock_, &buf, 1, NULL, &flags, &(this->ol_), UDPCompRoutine);

    wait = WSAWaitForMultipleEvents(1, &(ol_.hEvent), FALSE, INFINITE, TRUE);
    WSAResetEvent(ol_.hEvent);
    if(wait == WSA_WAIT_FAILED) {
        return 0;
    }
    return 1;
}

sock sock::operator=(sock right) {
    if(this == &right) {
        return *this;
    }
    addr_ = right.getAddr();
    bRecv_ = right.getRecv();
    bSend_ = right.getSent();
    sock_ = right.getSock();

    return *this;
}

void CALLBACK UDPCompRoutine(DWORD error, DWORD cbTransferred,
                        LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags) {

    sock* s = (sock*)lpOverlapped;
    char buf[PACKETSIZE];

    if(cbTransferred == 0 || error != 0) {
        WSAError(RD_ERROR);
    } else {
        strncpy(buf, s->packet_, PACKETSIZE);
        //process buffer, check message type, push onto correct buffer, signal buffer event.
        s->clrPacket();
    }

}
void CALLBACK UDPSendCompRoutine(DWORD error, DWORD cbTransferred,
                        LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags) {
    sock* s = (sock*)lpOverlapped;
    if(cbTransferred == 0 || error != 0) {
        WSAError(WR_ERROR);
    }
    s->clrPacket();
}
void CALLBACK TCPSendCompRoutine(DWORD error, DWORD cbTransferred,
                        LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags) {
    sock* s = (sock*)lpOverlapped;
    if(cbTransferred == 0 || error != 0) {
        WSAError(WR_ERROR);
    }
    s->clrPacket();
}
void CALLBACK TCPCompRoutine(DWORD error, DWORD cbTransferred,
                        LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags) {
    sock* s = (sock*)lpOverlapped;
    char buf[PACKETSIZE];

    if(cbTransferred == 0 || error != 0) {
        WSAError(RD_ERROR);
    } else {
        strncpy(buf, s->packet_, PACKETSIZE);
        //process buffer, check message type, push onto correct buffer, signal buffer event.
        s->clrPacket();
    }

}
