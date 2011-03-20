#ifndef CONNECTIONCONTROL_H
#define CONNECTIONCONTROL_H

#include <QString>
#include "../network/network.h"
#include "../network/socket.h"
#include "../network/tcpserver.h"
#include "../network/udpserver.h"

class tcpserver;
class udpserver;

class ConnectionControl {
public:
    explicit ConnectionControl();
    virtual ~ConnectionControl();

    bool startServer(int tcpPort, int udpPort);
    bool connectToServer(QString tcpIp, int tcpPort, int udpPort);

private:
    struct sock socket_;
    tcpserver *tcpServer_;
    udpserver *udpServer_;
};

#endif // CONNECTIONCONTROL_H
