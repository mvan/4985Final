/**
  * Class: ConnectionControl
  *
  * Description:
  * Main control of the application. This starts every threads, and
  * works between thread and GUI. It contains functionalities such as:
  * - start up the server
  * - connect to server
  * - send chat
  * - send/receive file
  * - stream audio
  * - start/stop microphone
  * - add and send playlist
  */
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
#include "../audio/audioin.h"

class ConnectionControl: public QObject {
    Q_OBJECT
public:
    explicit ConnectionControl();
    virtual ~ConnectionControl();

    /**
      * Function: startServer(int tcpPort, int udpPort
      *
      * @param tcpPort TCP port number
      * @param udpPort UDP port number
      *
      * @return bool The status of the server
      *
      * Description:
      * This function starts ther server thread.
      */
    bool startServer(int tcpPort, int udpPort);

    /**
      * Function: connectToServer(QString tcpIp, int tcpPort, int udpPort)
      *
      * @param tcpIp The server TCP IP address
      * @param tcpPort Server TCP port
      * @param udpPort Server UDP port
      *
      * @return bool True if succesfully connected, false otherwise
      *
      * Description:
      * This function tries to connect to a server, and join multicast group.
      * If cannot connect, the program will just run as a server.
      */
    bool connectToServer(QString tcpIp, int tcpPort, int udpPort);

    /**
      * Function: getFileName()
      *
      * @return QString Current filename
      *
      * Description:
      * This function returns the current file name.
      */
    QString getFileName();

signals:
    void listUpdate(char* fname);

public slots:

    /**
      * Slot:requestFT(char* fileName)
      *
      * Description:
      * Request a file from the list for transfer.
      */
    void requestFT(char* fileName);

    /**
      * Slot:startFTFromReq(char* fileName, char clientNo)
      *
      * Description:
      * Start transfer a requested file.
      */
    void startFTFromReq(char* fileName, char clientNo);

    /**
      * Slot:endFTOut()
      *
      * Description:
      * Manage output buffer. Read data from file, preparing for transfer.
      */
    void endFTOut();

    /**
      * Slot:endFTIn()
      *
      * Description:
      * Manage input buffer. Save the data to file.
      */
    void endFTIn();

    /**
      * Slot:requestStream(char* fileName)
      *
      * Description:
      * Send a request with a file name to the server for streaming.
      */
    void requestStream(char* fileName);

    /**
      * Slot:startStreamFromReq(char* fileName)
      *
      * Description:
      * Stream data from the requested file.
      */
    void startStreamFromReq(char* fileName);

    /**
      * Slot:endStreamOut()
      *
      * Description:
      * Stop receiving data for currently streaming file.
      */
    void endStreamOut();

    /**
      * Slot:connectionSlot(char* ipaddr)
      *
      * Description:
      * Notify the server about client connections.
      */
    void connectionSlot(char* ipaddr);

    /**
      * Slot:sendFilePacket(char*, char)
      *
      * Description:
      * Send a data packet of a file transfer.
      */
    void sendFilePacket(char*, char);

    /**
      * Slot:sendAudioPacket(char*)
      *
      * Description:
      * Send a data packet of a streaming audio.
      */
    void sendAudioPacket(char*);

    /**
      * Slot:sendChatPacket(char* packet)
      *
      * Description:
      * Send a data packet for chat.
      */
    void sendChatPacket(char* packet);

    /**
      * Slot:updateList(char* fname)
      *
      * Description:
      * Update playlist.
      */
    void updateList(char* fname);

    /**
      * Slot:addAudioFile(QString filename)
      *
      * Description:
      * Add file to the playlist. This file name will appears for others.
      */
    void addAudioFile(QString filename);

    /**
      * Slot:startMicStream()
      *
      * Description:
      * Send audio packet for a microphone.
      */
    void startMicStream();

    void endMicStream();

    void displayMB(QString msg);

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
    AudioSendThread* micThread_;
    audioin* micReader_;
    QList<sock> clientsSocket_;
};

#endif // CONNECTIONCONTROL_H
