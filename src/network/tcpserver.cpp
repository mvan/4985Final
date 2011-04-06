#include "tcpserver.h"
#include "socket.h"
#include "externs.h"
#include <QDebug>
void tcpserver::run() {
    fd_set readSet;
    listenSock_->TCPSocket_Init();
    listenSock_->TCPSocket_Bind(portNo);
    listenSock_->TCPSocket_Listen();
    initSelect();

    while(1) {

        readSet = allSet_;

        if((numReady_ = select(NULL, &readSet, NULL, NULL, NULL)) == SOCKET_ERROR) {
            continue;
        }

        if(FD_ISSET(listenSocket_, &readSet)) {
            if(addSelectSock() == 0) {
                continue;
            }
            Sleep(100);
            if((--numReady_) <= 0) {
                continue;
            }
        }
        for(int i = 0; i < FD_SETSIZE; ++i) {
            int nRead = 0;
            SOCKET s = 0;

            if((s = selectSocks_[i]) == 0) {
                continue;
            }
            if(FD_ISSET(s, &readSet)) {
                sock so(s);
                nRead = so.TCPRecv();
                if(nRead == 0) {
                    removeSelectSock(s);
                }
                if(nRead > 0) {
                    char pack[PACKETSIZE];
                    memcpy(pack, &(so.packet_[0]), PACKETSIZE);
                    if(ProcessTCPPacket(pack) == -1) {
                        emit(connectionRequest(pack+4));
                    }
                }
                if((--numReady_) <= 0) {
                    break;
                }
            }
        }
    }
}

void tcpserver::initSelect() {
    for(int i = 0; i < FD_SETSIZE; ++i) {
        selectSocks_[i] = 0;
    }

    listenSocket_ = listenSock_->getSock();

    FD_ZERO(&allSet_);
    FD_SET(listenSocket_, &allSet_);

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
            closesocket(s);
            return;
        }
    }
}

int tcpserver::ProcessTCPPacket(char* packet) {
    switch(packet[0]) {
        case MSG_CONN:
            return -1;
        case MSG_CONNBACK:
            ClientNum = packet[3];
            break;
        case MSG_ACK:
            //acknowledge a request...any request.
            break;
        case MSG_STREAMREQ:
            emit StreamReq(packet+4);
            break;
        case MSG_STREAMPAUSE:
        case MSG_STREAMCOMPLETE:
            audioinBuffer.bufferPacket(packet);
            break;
        case MSG_FTREQ:
            emit FTReq(packet+4, packet[3]);
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
        case MSG_MICOPEN:
            emit MicReq();
            break;
        default:
            return 0;
    }
    return 0;
}
