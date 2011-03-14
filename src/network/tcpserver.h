#ifndef TCPSERVER_H
#define TCPSERVER_H
#include <QMap>
#include "socket.h"
#include "server.h"
#include "network.h"
#include "errors.h"
#define MAX_CLIENTS 16
class tcpserver: public server {

    sock* listenSock_;
    sock clients_[MAX_CLIENTS];

    public:
        explicit tcpserver (){}
        virtual ~tcpserver(){}
        virtual void run();


};
#endif // TCPSERVER_H
