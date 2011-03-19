#ifndef CONNECTIONCONTROL_H
#define CONNECTIONCONTROL_H

#include <QString>
#include "../network/network.h"
#include "../network/socket.h"
#include "../network/tcpserver.h"
#include "../network/udpserver.h"

class ConnectionControl {
public:
    explicit ConnectionControl();
    virtual ~ConnectionControl();
    void startUDPServer(int port = UDPPORT);
    void startTCPServer(int port = TCPPORT);
    bool connectToServer(QString ip, int port = TCPPORT);
private:
    tcpserver* tcpserv_;
    udpserver* udpserv_;
    sock socket_;
};

#endif // CONNECTIONCONTROL_H
