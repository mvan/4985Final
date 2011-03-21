#ifndef UDPSERVERTHREAD_H
#define UDPSERVERTHREAD_H

#include <QThread>
#include "../network/udpserver.h"

class udpserver;

class UDPServerThread : public QThread {
    Q_OBJECT
public:
    explicit UDPServerThread(int port, QObject *parent = 0);
    virtual ~UDPServerThread();

    void run();

signals:

public slots:

private:
    udpserver *udpServer_;
    int port_;
};

#endif // UDPSERVERTHREAD_H
