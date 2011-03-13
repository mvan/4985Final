#ifndef SOCKET_H
#define SOCKET_H

#include <Winsock2.h>
#include "network.h"
#include "errors.h"
#define BUFSIZE 40960

class socket {
    private:
        SOCKET sock_;
        OVERLAPPED ol_;
        HANDLE closeEvent_;
        size_t bSend_;
        size_t bRecv_;
        char packet_[PACKETSIZE];
        struct sockaddr_in addr_;
        
    public:
    
        explicit socket():bSend_(0), bRecv_(0){
            closeEvent_ = CreateEvent(NULL, TRUE, FALSE, NULL);
        }
        virtual ~socket{
            closesocket(sock_);
        }
        
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
        void wait();
        
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
        
        //completion routines.
        friend void CALLBACK UDPCompRoutine(DWORD error, DWORD cbTransferred,
                                LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags);
        friend void CALLBACK TCPCompRoutine(DWORD error, DWORD cbTransferred,
                                LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags);
}

#endif
