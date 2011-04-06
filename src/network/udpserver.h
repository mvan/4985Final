#ifndef UDPSERVER_H
#define UDPSERVER_H
#include "socket.h"
#include "server.h"
#include "network.h"
#include "errors.h"
class udpserver: public server {
    Q_OBJECT
    private:
        sock* rdSock_;
        int portNo;
    public:
        explicit udpserver(int port):server(), portNo(port){
            rdSock_ = new sock();
        }
        virtual ~udpserver() {
            rdSock_->socket_close();
            delete rdSock_;
        }
        void ProcessUDPPacket(char* packet);
        virtual void run();
};
#endif
