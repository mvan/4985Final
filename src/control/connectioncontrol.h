#ifndef CONNECTIONCONTROL_H
#define CONNECTIONCONTROL_H

#include <QString>
#include <QObject>
#include <QStringList>
#include <winsock2.h>
#include "../network/network.h"
#include "../network/socket.h"
#include "../network/audiotransfer.h"
#include "../network/audiotransferin.h"
#include "../network/filetransfer.h"
#include "../network/filetransferin.h"
#include "../network/tcpserver.h"
#include "../network/udpserver.h"

class ConnectionControl: public QObject {
    Q_OBJECT
public:
    explicit ConnectionControl();
    virtual ~ConnectionControl();

    bool startServer(int tcpPort, int udpPort);
    bool connectToServer(QString tcpIp, int tcpPort);
    QString getFileName();

signals:
    void listUpdate(char* fname);

public slots:
    void requestFT(char* fileName);
    void startFTFromReq(char* fileName, char clientNo);
    void endFTOut();
    void endFTIn();
    void requestStream(char* fileName);
    void startStreamFromReq(char* fileName);
    void endStreamIn();
    void endStreamOut();
    void connectionSlot(char* ipaddr);
    void sendFilePacket(char*, char);
    void sendAudioPacket(char*);
    void sendChatPacket(char* packet);
    void updateList(char* fname);
    void addAudioFile(QString filename);

private:
    int tcpPort_;
    int udpPort_;
    int numConnections_;
    sock connections_[64];
    sock TCPSocket_;
    sock UDPSocket_;
    tcpserver* tcpServer_;
    udpserver* udpServer_;
    AudioReadThread* audioOutThread_;
    AudioWriteThread* audioInThread_;
    FileReadThread* fileOutThread_;
    FileWriteThread* fileInThread_;
    QList<sock> clientsSocket_;
};

#endif // CONNECTIONCONTROL_H
