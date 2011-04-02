#ifndef SOCKET_H
#define SOCKET_H
#include <winsock2.h>
#include "network.h"
#include <ws2tcpip.h>
#include "errors.h"
#define BUFSIZE 40960

class sock {

    public:
        WSAOVERLAPPED ol_;
        char packet_[PACKETSIZE];

    private:
        SOCKET sock_;
        size_t bSend_;
        size_t bRecv_;
        struct sockaddr_in addr_;
        char localaddr_[16];

    public:

        explicit sock():sock_(0), bSend_(0), bRecv_(0){}
        explicit sock(SOCKET socket):sock_(socket), bSend_(0), bRecv_(0) {}
        virtual ~sock() {}


        void TCPSocket_Init();
        void TCPSocket_Bind(int portNo);
        void TCPSocket_Listen();
        BOOL TCPSocket_Connect(char* servAddr, int portNo);
        SOCKET TCPSocket_Accept();

        void UDPSocket_Init();
        BOOL UDPSocket_Bind_Multicast(int portNo);

        int TCPSend();
        int UDPSend_Multicast();
        int TCPRecv();
        int UDPRecv_Multicast();

        void setLocalAddr() {
            struct sockaddr_in s;
            socklen_t size;
            getsockname(sock_, (struct sockaddr*)&s, &size);
            strcpy(localaddr_, inet_ntoa(s.sin_addr));
        }
        char* getLocalAddr() {
            return localaddr_;
        }

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
        void setSent(size_t send) {
            bSend_ = send;
        }
        size_t getRecv() {
            return bRecv_;
        }
        void setRecv(size_t recv) {
            bRecv_ = recv;
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
        void createOLEvent() {
            ol_.hEvent = WSACreateEvent();
        }
        void setPacket(char* packet) {
            memcpy(packet_, packet, PACKETSIZE);
        }
        void socket_close() {
            closesocket(sock_);
        }
};

//completion routines.
void CALLBACK UDPCompRoutine(DWORD error, DWORD cbTransferred,
                        LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags);
void CALLBACK sendCompRoutine(DWORD error, DWORD cbTransferred,
                        LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags);
void CALLBACK TCPCompRoutine(DWORD error, DWORD cbTransferred,
                        LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags);
#endif
