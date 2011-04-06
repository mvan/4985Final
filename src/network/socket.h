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
        /**
          * TCPSocket_Init()
          *
          * Description:
          * This function initializes a socket as a TCP socket.
          */
        void TCPSocket_Init();
        /**
          * TCPSocket_bind()
          *
          * @param portNo - the port to bind the socket to.
          *
          * Description:
          * This function binds a TCP socket to a specified port.
          */
        void TCPSocket_Bind(int portNo);
        /**
          * TCPSocket_Listen()
          *
          *
          * Description:
          * This function sets a TCP socket to the listen state.
          */
        void TCPSocket_Listen();
        /**
          * TCPSocket_Connect(char* servAddr, int portNo)
          *
          * @param char* servAddr - the server address to connect to.
          * @param porno* the port to connect over.
          *
          * @return BOOL - returns true on succes, true on failure
          *
          * Description:
          * This function sets a TCP socket to the listen state.
          */
        BOOL TCPSocket_Connect(char* servAddr, int portNo);
        /**
          * TCPSocket_Accept()
          *
          *
          * @return SOCKET - returns the newly accepted socket.
          *
          * Description:
          * This accepts a connection request from a listening a TCP socket.
          */
        SOCKET TCPSocket_Accept();
        /**
          * UDPSocket_Init()
          *
          *
          * Description:
          * Initializes a socket to be a UDP socket
          */
        void UDPSocket_Init(int portNo);
        /**
          * UDPSocket_Bind_Multicast()
          *
          * @return BOOL returns true on success, false if anything fails.
          *
          * Description:
          * binds a UDP socket to a port, and sets it up for multicasting.
          */
        BOOL UDPSocket_Bind_Multicast();
        /**
          * TCPSend()
          *
          * @return int returns the number of bytes sent.
          *
          * Description:
          * Sends a packet over a TCP socket..
          */
        int TCPSend();
        /**
          * UDPSend_Multicast()
          *
          * @return int returns the number of bytes sent.
          *
          * Description:
          * Multicasts a packet over a UDP socket.
          */
        int UDPSend_Multicast();
        /** TCPRecv()
        *
        * @return int returns the number of bytes read.
        *
        * Description:
        * Reads a packet off of a TCP socket.  */
        int TCPRecv();
        /** UDPRecv_Multicast()
        *
        * @return int returns the number of bytes read.
        *
        * Description:
        * Reads a packet off of a UDP multicast socket.  */
        int UDPRecv_Multicast();
        /** setLocalAddr()
        *
        *
        * Description:
        * Fills the localaddr_ member of the socket class with the
        * IP address of the computer the program is currently running on */
        void setLocalAddr() {
            struct sockaddr_in s;
            socklen_t size;
            getsockname(sock_, (struct sockaddr*)&s, &size);
            strcpy(localaddr_, inet_ntoa(s.sin_addr));
        }
        /** getLocalAddr()
        *
        *
        * @return the computer's local address
        *
        * Description:
        * getter for localaddr_ */
        char* getLocalAddr() {
            return localaddr_;
        }
        /** clrPacket()
        *
        *
        * Description:
        * clears the current socket's packet_ array.*/
        //inline functions
        void clrPacket() {
            mut_->lock();
            ZeroMemory(packet_, PACKETSIZE);
            mut_->unlock();
        }
        /** getSock()
        *
        * @return SOCKET the socket held in the current sock object
        *
        * Description:
        * getter for getSock*/
        SOCKET getSock() {
            return sock_;
        }
        /** setSock()
        *
        * @param sock the socket held in the current sock object
        *
        * Description:
        * getter for getSock*/
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
            memmove(&(packet_[0]), packet, PACKETSIZE);
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
