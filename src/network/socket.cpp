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
    bool reuseaddr = true;

    if ((sock_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) {
        WSAError(SOCK_ERROR);
    }
    if(setsockopt(sock_, SOL_SOCKET, SO_REUSEADDR, (char*)&reuseaddr, sizeof(bool)) == SOCKET_ERROR){
        WSAError(SOCK_ERROR);
    }
    if(setsockopt(sock_, SOL_SOCKET, SO_RCVBUF, (char*)&sizebuf, sizeof(int)) == SOCKET_ERROR){
        WSAError(SOCK_ERROR);
    }
    if(setsockopt(sock_, SOL_SOCKET, SO_SNDBUF, (char*)&sizebuf, sizeof(int)) == SOCKET_ERROR){
        WSAError(SOCK_ERROR);
    }
}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: TCPSocket_Bind
--
-- DATE: Feb 19, 2011
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Duncan Donaldson
--
-- PROGRAMMER: Duncan Donaldson
--
-- INTERFACE: void TCPSocket_Bind(int portNo)
--				PortNo - the port number to bind to.
--
-- RETURNS: void
--
-- NOTES:
-- binds a TCP socket to a port, and sets it to the listen state.
----------------------------------------------------------------------------------------------------------------------*/
void sock::TCPSocket_Bind(int portNo) {

    ZeroMemory(&addr_, sizeof(struct sockaddr_in));
    addr_.sin_family = AF_INET;
    addr_.sin_port = htons(portNo);
    addr_.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock_, (struct sockaddr *)&addr_, sizeof(struct sockaddr_in)) == SOCKET_ERROR) {
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

    if (connect (sock_, (struct sockaddr *)&addr_, sizeof(struct sockaddr_in)) == SOCKET_ERROR) {
        return FALSE;
    }
    return TRUE;

}

SOCKET sock::TCPSocket_Accept() {

    SOCKET s;

    if((s = accept(sock_, NULL, NULL)) == INVALID_SOCKET) {
        return 0;
    }
    return s;

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
void sock::UDPSocket_Init(int port) {
    int sizebuf = BUFSIZE;
    bool reuseaddr = true;
    this->udpPort = port;
    if ((sock_ = WSASocket(PF_INET, SOCK_DGRAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET) {
        WSAError(SOCK_ERROR);
    }

    if(setsockopt(sock_, SOL_SOCKET, SO_REUSEADDR, (char*)&reuseaddr, sizeof(bool)) == SOCKET_ERROR) {
        WSAError(SOCK_ERROR);
    }
    if(setsockopt(sock_, SOL_SOCKET, SO_RCVBUF, (char*)&sizebuf, sizeof(int)) == SOCKET_ERROR) {
        WSAError(SOCK_ERROR);
    }
    if(setsockopt(sock_, SOL_SOCKET, SO_SNDBUF, (char*)&sizebuf, sizeof(int)) == SOCKET_ERROR) {
        WSAError(SOCK_ERROR);
    }
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
BOOL sock::UDPSocket_Bind_Multicast() {

    struct ip_mreq mc_addr;
    int ttl = MULTICAST_TTL;
    BOOL loopback = FALSE;

    ZeroMemory(&addr_, sizeof(struct sockaddr_in));
    addr_.sin_family = AF_INET;
    addr_.sin_port = htons(udpPort);
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
    DWORD nRead;
    mut_->lock();
    nRead = send(sock_, packet_, PACKETSIZE, 0);
    mut_->unlock();
    return nRead;
}

int sock::UDPSend_Multicast() {
    WSABUF buf;

    buf.buf = packet_;
    buf.len = PACKETSIZE;
    addr_.sin_family = AF_INET;
    addr_.sin_addr.s_addr = inet_addr(MULTICAST_ADDR);
    addr_.sin_port = htons(udpPort);
    WSASendTo(this->sock_, &buf, 1, NULL, 0, (struct sockaddr*)&addr_,
                        sizeof(addr_), &(this->ol_), sendCompRoutine);
    return 1;
}

int sock::TCPRecv() {
    int nRead;
    int total = 0, toRead = PACKETSIZE;

    mut_->lock();
    while(toRead > 0) {
        if((nRead = recv(sock_, packet_+total, toRead, 0)) == 0) {
            return 0;
        }
        toRead -= nRead;
        total += nRead;
    }
    mut_->unlock();

    return total;
}

int sock::UDPRecv_Multicast() {
    DWORD flags = 0, wait;
    WSABUF buf;

    buf.buf = packet_;
    buf.len = PACKETSIZE;
    createOLEvent();
    WSARecv(sock_, &buf, 1, NULL, &flags, &(this->ol_), UDPCompRoutine);
    wait = WSAWaitForMultipleEvents(1, &(ol_.hEvent), FALSE, INFINITE, TRUE);
    CloseHandle(ol_.hEvent);
    if(wait == WSA_WAIT_FAILED) {
        return 0;
    }
    return 1;
}

void CALLBACK UDPCompRoutine(DWORD error, DWORD cbTransferred,
                        LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags) {

    sock* s = (sock*)lpOverlapped;
    char buf[PACKETSIZE];

    if(cbTransferred == 0 || error != 0) {
        WSAError(RD_ERROR);
    } else {
        memcpy(buf, s->packet_, PACKETSIZE);
        ProcessUDPPacket(buf);
    }

}

void CALLBACK sendCompRoutine(DWORD error, DWORD cbTransferred,
                        LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags) {
    sock* s = (sock*)lpOverlapped;
    if(cbTransferred == 0 || error != 0) {
        WSAError(WR_ERROR);
    }
}
