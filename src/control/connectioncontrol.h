#ifndef CONNECTIONCONTROL_H
#define CONNECTIONCONTROL_H

#include <QString>

#include "../network/socket.h"
#include "../network/tcpserver.h"
#include "../network/udpserver.h"

class tcpserver;
class udpserver;

class ConnectionControl {
public:
    ConnectionControl();
    ~ConnectionControl();

    bool startServer(int tcpPort, int udpPort);
    bool connectToServer(QString ip, int port);

private:
    struct sock socket_;
    tcpserver *tcpServer_;
    udpserver *udpServer_;
};

#endif // CONNECTIONCONTROL_H
