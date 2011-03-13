#ifndef UDPSERVER_H
#define UDPSERVER_H
#include "socket.h"
#include "server.h"
#include "network.h"
#include "errors.h"
class udpserver: public server {
    private:
        socket* rdSock_;
        
    public:
        explicit udpserver():server(){}
        virtual ~udpserver() {
            delete rdSock_;
            delete wrSock_;
        }
        
        virtual void run();
}
#endif