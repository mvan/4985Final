#ifndef NETWORK_H
#define NETWORK_H
#include <Winsock2.h>
#include "helpers.h"
#define MULTICAST_ADDR "234.98.98.98"
#define MULTICAST_TTL 2
#define TCPPORT 7898
#define UDPPORT 7899
#define PACKETSIZE 4096
#define BUFSIZE 40960
#define SOCK_ERROR 1
#define RD_ERROR 2
#define WR_ERROR 3
#define EVT_ERROR 4
#define THREAD_ERROR 5
#define ALLOC_ERROR 6
#define FILE_ERROR 7

void WinsockInit();
void WinsockCleanup();
void WSAError(int error);

class socket {
    private:
        SOCKET sock_;
        OVERLAPPED ol_;
        size_t bSend_;
        size_t bRecv_;
        char packet_[PACKETSIZE];
        struct sockaddr_in addr_;
        
    public:
    
        explicit socket():bSend_(0), bRecv_(0){}
        virtual ~socket{}
        
        void TCPSocket_Init();
        void TCPSocket_Bind();
        void TCPSocket_Listen();
        void TCPSocket_Connect(char* servAddr_);
        void UDPSocket_Init();
        void UDPSocket_Bind_Multicast();
        void TCPSend(char* buf);
        void UDPSend(char* buf);
        void TCPRecv(char* buf);
        void UDPRecv(char* buf);
        
        //inline functions
        void clrPacket() {
            ZeroMemory(packet_, PACKETSIZE);
        }
        SOCKET getSock() {
            return sock_;
        }
        void setSock(SOCKET sock) {
            sock_ = sock;
        }
        size_t getSent() {
            return bSend_;
        }
        size_t getRecv() {
            return bRecv_;
        }
        struct sockaddr_in getAddr() {
            return addr_;
        }
        OVERLAPPED getOverlapped() {
            return ol_;
        }
        void setOverlapped(OVERLAPPED ol) {
            ol_ = ol;
        }
        
    private:
        //completion routines.
        void CALLBACK UDPCompRoutine(DWORD error, DWORD cbTransferred,
                                LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags);
        void CALLBACK TCPCompRoutine(DWORD error, DWORD cbTransferred,
                                LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags);
}

#endif
