#ifndef SOCKET_H
#define SOCKET_H

#include <winsock2.h>
#include "network.h"
#include <ws2tcpip.h>
#include "errors.h"
#define BUFSIZE 40960

struct sock {

    WSAOVERLAPPED ol_;
    char packet_[PACKETSIZE];
    
    private:
        SOCKET sock_;
        HANDLE closeEvent_;
        size_t bSend_;
        size_t bRecv_;
        struct sockaddr_in addr_;
        
    public:
    
        explicit sock():bSend_(0), bRecv_(0){}
        explicit sock(SOCKET socket):sock_(socket), bSend_(0), bRecv_(0) {}

        virtual ~sock() {
            closesocket(sock_);
        }
        
        void TCPSocket_Init();
        BOOL TCPSocket_Bind(int portNo);
        void TCPSocket_Listen();
        BOOL TCPSocket_Connect(char* servAddr, int portNo);
        sock TCPSocket_Accept();
        void UDPSocket_Init();
        BOOL UDPSocket_Bind_Multicast(int portNo);
        int TCPSend();
        int UDPSend_Multicast();
        int TCPRecv();
        int UDPRecv_Multicast();
        sock operator=(sock right);

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
        WSAOVERLAPPED getOverlapped() {
            return ol_;
        }
        void setOverlapped(WSAOVERLAPPED ol) {
            ol_ = ol;
        }
};

//completion routines.
void CALLBACK UDPCompRoutine(DWORD error, DWORD cbTransferred,
                        LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags);
void CALLBACK UDPSendCompRoutine(DWORD error, DWORD cbTransferred,
                        LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags);
void CALLBACK TCPCompRoutine(DWORD error, DWORD cbTransferred,
                        LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags);
void CALLBACK TCPSendCompRoutine(DWORD error, DWORD cbTransferred,
                        LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags);
#endif
