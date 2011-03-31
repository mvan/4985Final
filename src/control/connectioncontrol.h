#ifndef CONNECTIONCONTROL_H
#define CONNECTIONCONTROL_H

#include <QString>
#include <QObject>
#include <QStringList>
#include <winsock2.h>
#include "udpserverthread.h"
#include "tcpserverthread.h"
#include "../network/network.h"
#include "../network/socket.h"
#include "../network/audiotransfer.h"
#include "../network/audiotransferin.h"
#include "../network/filetransfer.h"
#include "../network/filetransferin.h"

class ConnectionControl: public QObject {
    Q_OBJECT
public:
    explicit ConnectionControl();
    virtual ~ConnectionControl();

    bool startServer(int tcpPort, int udpPort);
    bool connectToServer(QString tcpIp, int tcpPort);
    TCPServerThread* getTCPServerThread();
    UDPServerThread* getUDPServerThread();
    sock getTCPSocket();
    sock getUDPSocket();
    QString getFileName();
    HANDLE openFile(QString fName);
public slots:
    void incomingFT();
    void startFT(QString fName);
    void endFTOut();
    void endFTIn();
    void incomingStream();
    void startStream();
    void endStreamIn();
    void endStreamOut();
    void connectionSlot(char* ipaddr);

private:
    int tcpPort_;
    int udpPort_;
    sock TCPSocket_;
    sock UDPSocket_;
    UDPServerThread *udpServerThread_;
    TCPServerThread *tcpServerThread_;
    AudioReadThread* audioOutThread_;
    AudioWriteThread* audioInThread_;
    FileReadThread* fileOutThread_;
    FileWriteThread* fileInThread_;
};

#endif // CONNECTIONCONTROL_H
