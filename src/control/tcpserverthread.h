#ifndef TCPSERVERTHREAD_H
#define TCPSERVERTHREAD_H

#include <QThread>

#include "../network/tcpserver.h"

class tcpserver;

class TCPServerThread : public QThread {
    Q_OBJECT
public:
    explicit TCPServerThread(int port, QObject *parent = 0);
    virtual ~TCPServerThread();

    void run();

signals:

public slots:

private:
    tcpserver *tcpServer_;
    int port_;
};

#endif // TCPSERVERTHREAD_H
