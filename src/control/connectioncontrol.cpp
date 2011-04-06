#include "connectioncontrol.h"
#include "../network/externs.h"
#include "../audio/audioin.h"
#include <winsock2.h>
#include <QFileDialog>
#include <QMessageBox>
#include <QList>
bool transferring = false;
bool streamingOut = false;
char ClientNum = 0;
ConnectionControl::ConnectionControl(): numConnections_(0){

}

ConnectionControl::~ConnectionControl() {

}

bool ConnectionControl::startServer(int tcpPort, int udpPort) {
    udpServer_ = new udpserver(udpPort);
    udpServer_->start();
    tcpServer_ = new tcpserver(tcpPort);
    tcpServer_->start();

    audioInThread_ = new AudioWriteThread();
    audioInThread_->start();

    //CONNECTION SIGNAL AND SLOT
    connect(tcpServer_, SIGNAL(connectionRequest(char*)), this,
            SLOT(connectionSlot(char*)), Qt::QueuedConnection);

    //FILE TRANSFER SIGNALS AND SLOTS
    connect(tcpServer_, SIGNAL(FTReq(char*, char)), this,
            SLOT(startFTFromReq(char*, char)), Qt::QueuedConnection);

    //LIST SIGNALS AND SLOTS
    connect(tcpServer_, SIGNAL(updateList(char*)), this,
            SLOT(updateList(char*)), Qt::QueuedConnection);

    //STREAM SIGNALS AND SLOTS
    connect(tcpServer_, SIGNAL(StreamReq(char*)), this,
            SLOT(startStreamFromReq(char*)), Qt::QueuedConnection);
    return true;
}

bool ConnectionControl::connectToServer(QString tcpIp, int tcpPort, int udpPort) {
    char connectionPacket[PACKETSIZE];
    tcpPort_ = tcpPort;
    UDPSocket_.UDPSocket_Init(udpPort);
    TCPSocket_.TCPSocket_Init();
    if(TCPSocket_.TCPSocket_Connect(tcpIp.toAscii().data(), tcpPort) == TRUE) {
        TCPSocket_.setLocalAddr();
        mkPacket(connectionPacket, MSG_CONN, PACKETSIZE, 0, TCPSocket_.getLocalAddr());
        TCPSocket_.setPacket(connectionPacket);
        TCPSocket_.TCPSend();
        TCPSocket_.clrPacket();
        return true;
    }
    TCPSocket_.socket_close();
    TCPSocket_.setSock(0);
    return false;
}

void ConnectionControl::connectionSlot(char* ipaddr) {
    QMessageBox notification;
    QString msg;
    char buf[PACKETSIZE];

    msg.append("Remote client connected at: ");
    msg.append(ipaddr);
    msg.append(", establishing connection to their server.");
    notification.setText(msg);
    if(TCPSocket_.getSock() != 0) {
        connections_[numConnections_].TCPSocket_Init();
        connections_[numConnections_].TCPSocket_Connect(ipaddr, tcpPort_);
        connections_[numConnections_].clrPacket();
        mkPacket(buf, MSG_CONNBACK, 0, numConnections_, (char*)"");
        connections_[numConnections_].setPacket(buf);
        connections_[numConnections_].TCPSend();
        numConnections_++;
    } else {
        TCPSocket_.TCPSocket_Init();
        TCPSocket_.TCPSocket_Connect(ipaddr, tcpPort_);
        TCPSocket_.clrPacket();
        mkPacket(buf, MSG_CONNBACK, 0, numConnections_, (char*)"");
        TCPSocket_.setPacket(buf);
        TCPSocket_.TCPSend();
    }
    notification.exec();
}

QString ConnectionControl::getFileName() {
    return QFileDialog::getSaveFileName(0, "Select a File");
}

void ConnectionControl::requestFT(char* fileName) {
    char packet[PACKETSIZE];
    QString fname;
    if(TCPSocket_.getSock() == 0) {
        QMessageBox m;
        m.setText(QString("You are not connected to a server."));
        m.exec();
        return;
    }
    if(transferring == true) {
        QMessageBox m;
        m.setText(QString("You are already transferring a file. Cannot start a new transfer."));
        m.exec();
        return;
    }
    if(fileName[0] == 0) {
        QMessageBox m;
        m.setText(QString("No file selected."));
        m.exec();
        return;
    }
    mkPacket(packet, MSG_FTREQ, strlen(fileName), ClientNum, fileName);

    fname = getFileName();
    if(fname.size() == 0) {
        QMessageBox m;
        m.setText(QString("No save file selected, aborting transfer."));
        m.exec();
        return;
    }
    fileInThread_ = new FileWriteThread(fname);
    fileInThread_->start();

    connect(fileInThread_, SIGNAL(endFT()), this,
            SLOT(endFTIn()), Qt::QueuedConnection);
    if(TCPSocket_.getSock() != 0) {
        TCPSocket_.clrPacket();
        TCPSocket_.setPacket(packet);
        TCPSocket_.TCPSend();
    } else {
        for(int i = 0; i < numConnections_; ++i) {
            TCPSocket_.clrPacket();
            TCPSocket_.setPacket(packet);
            TCPSocket_.TCPSend();
        }
    }
    transferring = true;
}

void ConnectionControl::startFTFromReq(char* fileName, char clientNo) {
    if(transferring == true) {
        return;
    }
    fileOutThread_ = new FileReadThread(QString(fileName), clientNo);
    //FILE TRANSFER SIGNALS AND SLOTS
    connect(fileOutThread_, SIGNAL(sendTCPPacket(char*, char)), this,
            SLOT(sendFilePacket(char*, char)), Qt::QueuedConnection);
    connect(fileOutThread_, SIGNAL(endFT()), this,
            SLOT(endFTOut()), Qt::QueuedConnection);
    fileOutThread_->start();
    transferring = true;
}

void ConnectionControl::endFTOut() {
    Sleep(100);
    disconnect(fileOutThread_, SIGNAL(sendTCPPacket(char*, char)), this,
            SLOT(sendFilePacket(char*, char)));
    disconnect(fileOutThread_, SIGNAL(endFT()), this,
            SLOT(endFTOut()));
    transferring = false;
    fileOutThread_->terminate();
    delete fileOutThread_;
    fileOutThread_= NULL;
}

void ConnectionControl::endFTIn() {
    disconnect(fileInThread_, SIGNAL(endFT()), this,
            SLOT(endFTIn()));
    fileInThread_->terminate();
    delete fileInThread_;
    fileInThread_ = NULL;
    transferring = false;
}

void ConnectionControl::startStreamFromReq(char* fName) {
   if(audioOutThread_ != 0) {
        audioOutThread_->terminate();
        delete audioOutThread_;
        audioOutThread_ = NULL;
        audiooutBuffer.queue.clear();
   }
   audioOutThread_ = new AudioReadThread(QString(fName));
   connect(audioOutThread_, SIGNAL(sendUDPPacket(char*)), this,
           SLOT(sendAudioPacket(char*)), Qt::QueuedConnection);
   connect(audioOutThread_, SIGNAL(endStream()), this,
           SLOT(endStreamOut()), Qt::QueuedConnection);
   audioOutThread_->start();
   streamingOut = true;
}

void ConnectionControl::startMicStream(){
    if(streamingOut == true) {
        QMessageBox m;
        m.setText(QString("Audio channel already open.\n Cannot open microphone."));
        m.exec();
        return;
    }
    micThread_ = new AudioSendThread();
    micReader_ = new audioin();
    micReader_->setupParams();
    micReader_->createAudioDev();
    connect(micThread_, SIGNAL(sendPacket(char*)), this,
            SLOT(sendAudioPacket(char*)), Qt::QueuedConnection);
    micThread_->start();
    streamingOut = true;
}

void ConnectionControl::endMicStream() {
    disconnect(micThread_, SIGNAL(sendPacket(char*)), this,
            SLOT(sendAudioPacket(char*)));
    if(micThread_ != NULL) {
        micThread_->terminate();
        delete micThread_;
        delete micReader_;
        micThread_ = NULL;
        micReader_ = NULL;
        streamingOut = false;
    }
}

void ConnectionControl::requestStream(char* fileName) {
    char packet[PACKETSIZE];
    if(TCPSocket_.getSock() == 0) {
        QMessageBox m;
        m.setText(QString("You are not connected to a server."));
        m.exec();
        return;
    }
    if(fileName[0] == 0) {
        QMessageBox m;
        m.setText(QString("No file selected."));
        m.exec();
        return;
    }

    mkPacket(packet, MSG_STREAMREQ, strlen(fileName), ClientNum, fileName);
    TCPSocket_.clrPacket();
    TCPSocket_.setPacket(packet);
    TCPSocket_.TCPSend();
}

void ConnectionControl::endStreamOut() {
    disconnect(audioOutThread_, SIGNAL(sendUDPPacket(char*)), this,
            SLOT(sendAudioPacket(char*)));
    disconnect(audioOutThread_, SIGNAL(endStream()), this,
            SLOT(endStreamOut()));
    if(audioOutThread_ != NULL) {
        audioOutThread_->terminate();
        delete audioOutThread_;
        audioOutThread_ = NULL;
    }
    streamingOut = false;
}

void ConnectionControl::sendFilePacket(char* packet, char req) {
    if(TCPSocket_.getSock() != 0) {
        TCPSocket_.clrPacket();
        TCPSocket_.setPacket(packet);
        TCPSocket_.TCPSend();
    }
    connections_[(int)req].clrPacket();
    connections_[(int)req].setPacket(packet);
    connections_[(int)req].TCPSend();
}

void ConnectionControl::sendAudioPacket(char* packet){
    UDPSocket_.clrPacket();
    UDPSocket_.setPacket(packet);
    UDPSocket_.UDPSend_Multicast();
}

void ConnectionControl::sendChatPacket(char* packet) {
    if(TCPSocket_.getSock() != 0) {
        TCPSocket_.clrPacket();
        TCPSocket_.setPacket(packet);
        TCPSocket_.TCPSend();
    }
    for(int i = 0; i <= numConnections_; ++i) {
        connections_[i].clrPacket();
        connections_[i].setPacket(packet);
        connections_[i].TCPSend();
    }
}

void ConnectionControl::updateList(char* fname) {
    emit listUpdate(fname);
}

void ConnectionControl::addAudioFile(QString filename) {

    char buf[PACKETSIZE];

    mkPacket(buf, MSG_LIST, filename.size(),
                        0, filename.toAscii().data());
    if(TCPSocket_.getSock() != 0) {
        TCPSocket_.clrPacket();
        TCPSocket_.setPacket(buf);
        TCPSocket_.TCPSend();
    }
    for(int i = 0; i <= numConnections_; ++i) {
        connections_[i].clrPacket();
        connections_[i].setPacket(buf);
        connections_[i].TCPSend();
    }
}
