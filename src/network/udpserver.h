#ifndef UDPSERVER_H
#define UDPSERVER_H
#include "socket.h"
#include "server.h"
#include "network.h"
#include "errors.h"
class udpserver: public server {
    private:
        sock* rdSock_;
        
    public:
        explicit udpserver():server(){
            rdSock_ = new sock();
        }
        virtual ~udpserver() {
            rdSock_->socket_close();
            delete rdSock_;
        }
        
        virtual void run(int portNo = UDPPORT);
};
#endif
