#ifndef TCPSERVER_H
#define TCPSERVER_H
#include <QMap>
#include "socket.h"
#include "server.h"
#include "network.h"
#include "errors.h"
#define MAX_CLIENTS 16
class tcpserver: public server {

    private:
        sock* listenSock_;
        sock socks_[FD_SETSIZE];
        SOCKET selectSocks_[FD_SETSIZE];
        SOCKET maxSock_;
        fd_set readySet_;
        fd_set allSet_;
        int numSocks_;
        int numReady_;
        int sockIndex_;

    public:
        explicit tcpserver ():numSocks_(0), numReady_(0), sockIndex_(-1){}
        virtual ~tcpserver(){}
        virtual void initSelect();
        int addSelectSock();
        virtual void run(int portNo = TCPPORT);


};
#endif // TCPSERVER_H
