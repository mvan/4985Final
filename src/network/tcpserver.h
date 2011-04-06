#ifndef TCPSERVER_H
#define TCPSERVER_H
#include <QMap>
#include <QList>
#include <QThread>
#include <QString>
#include "socket.h"
#include "server.h"
#include "network.h"
#include "errors.h"
#define MAX_CLIENTS 16
class tcpserver: public server {
    Q_OBJECT
    private:
        sock* listenSock_;
        SOCKET listenSocket_;
        sock socks_[FD_SETSIZE];
        SOCKET selectSocks_[FD_SETSIZE];
        fd_set allSet_;
        int numReady_;
        int portNo;

    public:
        explicit tcpserver (int port = TCPPORT): numReady_(0), portNo(port){
            listenSock_ = new sock();
        }
        virtual ~tcpserver(){
            listenSock_->socket_close();
            delete listenSock_;
        }
        virtual void initSelect();
        SOCKET addSelectSock();
        void removeSelectSock(SOCKET s);
        virtual void run();
        int ProcessTCPPacket(char* packet);

    signals:
        void FTReq(char*, char);
        void StreamReq(char*);
        void connectionRequest(char* hostaddr);
        void updateList(char* fname);
        void MicReq();
};
#endif // TCPSERVER_H
