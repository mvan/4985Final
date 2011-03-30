#ifndef TCPSERVER_H
#define TCPSERVER_H
#include <QMap>
#include <QList>
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
        fd_set readySet_;
        fd_set allSet_;
        int numReady_;
        QList<sock> currentClients_;

    public:
        explicit tcpserver (): numReady_(0){
            listenSock_ = new sock();
        }
        virtual ~tcpserver(){
            delete listenSock_;
        }
        virtual void initSelect();
        int addSelectSock();
        virtual void run(int portNo = TCPPORT);
        QList<sock> getAllClients();

};
#endif // TCPSERVER_H
