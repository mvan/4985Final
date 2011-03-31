#ifndef CONNECTIONCONTROL_H
#define CONNECTIONCONTROL_H

#include <QString>
#include <QObject>
#include "udpserverthread.h"
#include "tcpserverthread.h"
#include "../network/network.h"
#include "../network/socket.h"

class ConnectionControl: public QObject {
    Q_OBJECT
public:
    explicit ConnectionControl();
    virtual ~ConnectionControl();

    bool startServer(int tcpPort, int udpPort);
    bool connectToServer(QString tcpIp, int tcpPort, int udpPort);
    TCPServerThread* getTCPServerThread();
    sock getTCPSocket();
    sock getUDPSocket();

public slots:
    void startFT();
    void endFT();
    void startStream();
    void endStream();
private:
    sock TCPSocket_;
    sock UDPSocket_;
    UDPServerThread *udpServerThread_;
    TCPServerThread *tcpServerThread_;
};

#endif // CONNECTIONCONTROL_H
