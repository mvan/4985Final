#include "tcpserver.h"
#include "socket.h"
#include "externs.h"
#include <QDebug>
void tcpserver::run() {

    listenSock_->TCPSocket_Init();
    listenSock_->TCPSocket_Bind(portNo);
    listenSock_->TCPSocket_Listen();
    initSelect();

    while(1) {

        FD_ZERO(&readySet_);
        this->readySet_ = this->allSet_;

        if((numReady_ = select(NULL, &readySet_, NULL, NULL, NULL)) == SOCKET_ERROR) {
            continue;
        }

        if(FD_ISSET(listenSock_->getSock(), &readySet_)) {
            addSelectSock();
            Sleep(100);
            if(--numReady_ <= 0) {
                continue;
            }
        }

        for(int i = 0; i < FD_SETSIZE; ++i) {
            int nRead = 0;
            SOCKET s;

            if((s = selectSocks_[i]) < 0) {
                continue;
            }
            if(FD_ISSET(s, &readySet_)) {
                sock so(s);
                nRead = so.TCPRecv();
                if(nRead == 0) {
                    removeSelectSock(s);
                }
                if(nRead > 0) {
                    char pack[PACKETSIZE];
                    memcpy(pack, so.packet_, PACKETSIZE);
                    if(ProcessTCPPacket(pack) == -1) {
                        emit(connectionRequest(pack+4));
                    }
                }
            }
            if(--numReady_ <= 0) {
                break;
            }
        }
    }
}

void tcpserver::initSelect() {

    for(int i = 0; i < FD_SETSIZE; ++i) {
        selectSocks_[i] = -1;
    }

    memset(&(selectSocks_[0]), 0, FD_SETSIZE);

    FD_ZERO(&allSet_);
    FD_SET(listenSock_->getSock(), &allSet_);

}

SOCKET tcpserver::addSelectSock() {

    int i;

    SOCKET s = listenSock_->TCPSocket_Accept();
    if(s <= 0) {
        WSAError(SOCK_ERROR);
    }

    for(i = 0; i < FD_SETSIZE; ++i) {
        if(selectSocks_[i] == 0) {
            FD_SET(s, &allSet_);
            selectSocks_[i] = s;
            currentClients_.push_back(sock(s));
            break;
        }
    }
    return s;
}

void tcpserver::removeSelectSock(SOCKET s) {

    for(int i = 0; i < FD_SETSIZE; ++i) {
        if(selectSocks_[i] == s) {
            FD_CLR(s, &allSet_);
            selectSocks_[i] = 0;
            currentClients_.removeAt(i);
            closesocket(s);
            return;
        }
    }
}

QList<sock> tcpserver::getAllClients() {
    return currentClients_;
}

int tcpserver::ProcessTCPPacket(char* packet) {
    switch(packet[0]) {
        case MSG_CONN:
            return -1;
        case MSG_ACK:
            //acknowledge a request...any request.
            break;
        case MSG_STREAMREQ:
            //open a new incoming stream.
            break;
        case MSG_STREAMPAUSE:
        case MSG_STREAMCOMPLETE:
            audioinBuffer.bufferPacket(packet);
            break;
        case MSG_FTREQ:
            emit FTReq(packet+4);
            break;
        case MSG_FTCOMPLETE:
        case MSG_FT:
            fileinBuffer.bufferPacket(packet);
            break;
        case MSG_CHAT:
            chatinBuffer.bufferPacket(packet);
            break;
        case MSG_LISTREQ:
            //request a file list.
            break;
        case MSG_LIST: //recv a playlist item
            emit updateList(packet+4);
            break;
        default:
            return 0;
    }
    return 0;
}

void tcpserver::sendPacket(sock socket) {
    socket.TCPSend();
}

bool tcpserver::addAudioFile(QString filename) {

    for(int i = 0; i < FD_SETSIZE; ++i) {
        if(selectSocks_[i] != 0) {

            char buf[PACKETSIZE];
            sock socket(selectSocks_[i]);

            mkPacket(buf, MSG_LIST, filename.size(),
                                0x00, filename.toAscii().data());
            socket.clrPacket();
            socket.setPacket(buf);
            socket.TCPSend();
        }
    }
    return true;
}
