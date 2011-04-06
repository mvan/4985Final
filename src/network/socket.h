#ifndef SOCKET_H
#define SOCKET_H
#include <winsock2.h>
#include <ws2tcpip.h>
#include <QMutex>
#include "network.h"
#include "errors.h"
#define BUFSIZE 40960

class sock {

    public:
        WSAOVERLAPPED ol_;
        char packet_[PACKETSIZE];
        QMutex* mut_;
        WSABUF buffer_;


    private:
        SOCKET sock_;
        size_t bSend_;
        size_t bRecv_;
        struct sockaddr_in addr_;
        char localaddr_[16];
        int udpPort;
        bool mc_;

    public:

        explicit sock():sock_(0), bSend_(0), bRecv_(0), mc_(false){
            mut_ = new QMutex();
            buffer_.buf = packet_;
            buffer_.len = PACKETSIZE;
        }
        explicit sock(SOCKET socket):sock_(socket), bSend_(0), bRecv_(0) {
            mut_ = new QMutex();
        }
        virtual ~sock() {
            delete mut_;
        }

        void TCPSocket_Init();
        void TCPSocket_Bind(int portNo);
        void TCPSocket_Listen();
        BOOL TCPSocket_Connect(char* servAddr, int portNo);
        SOCKET TCPSocket_Accept();

        void UDPSocket_Init(int portNo);
        BOOL UDPSocket_Bind_Multicast();

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
            mut_->lock();
            ZeroMemory(packet_, PACKETSIZE);
            mut_->unlock();
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
            memcpy(&(packet_[0]), packet, PACKETSIZE);
        }
        void socket_close() {
            struct ip_mreq mc_addr;
            if(mc_ == true) {
                mc_addr.imr_multiaddr.s_addr = inet_addr(MULTICAST_ADDR);
                mc_addr.imr_interface.s_addr = INADDR_ANY;

                setsockopt(sock_, IPPROTO_IP, IP_DROP_MEMBERSHIP,
                           (char *)&mc_addr, sizeof(mc_addr));
            }
            closesocket(sock_);
        }
};

//completion routines.
void CALLBACK UDPCompRoutine(DWORD error, DWORD cbTransferred,
                        LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags);
void CALLBACK sendCompRoutine(DWORD error, DWORD cbTransferred,
                        LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags);
#endif
