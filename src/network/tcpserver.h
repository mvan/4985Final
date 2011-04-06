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
        /**
          * initSelect()
          *
          *
          * Description:
          * This function initiazlizes the tcpserver for select.
          */
        virtual void initSelect();
        /**
          * addSelectSock()
          *
          * @return returns the newly accepted and added socket.
          *
          * Description:
          * accepts an incoming connection request and adds the socket
          * to the select sets.
          */
        SOCKET addSelectSock();
        /**
          * removeSelectSock(SOCKET s)
          *
          * @param the socket to be removed from the sets.
          *
          * Description:
          * removes a socket from the select sets.
          */
        void removeSelectSock(SOCKET s);
        /**
          * run()
          *
          *
          * Description:
          * Runs the TCP server.
          */
        virtual void run();
        /**
          * ProcessTCPPacket(char* packet)
          *
          * @param the packet to be processed.
          *
          * @return -1 if the packet is a connection notification, 1 otherwise.
          *
          * Description:
          * Handles a received TCP packet.
          */
        int ProcessTCPPacket(char* packet);

    signals:
        void FTReq(char*, char);
        void StreamReq(char*);
        void connectionRequest(char* hostaddr);
        void updateList(char* fname);
        void MicReq();
};
#endif // TCPSERVER_H
