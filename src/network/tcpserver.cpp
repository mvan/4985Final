#include "tcpserver.h"
#include "socket.h"
void tcpserver::run(int portNo) {

    TIMEVAL tv;

    listenSock_->TCPSocket_Init();
    listenSock_->TCPSocket_Bind(portNo);
    listenSock_->TCPSocket_Listen();
    initSelect();

    tv.tv_sec = INFINITE;
    tv.tv_usec = 0;

    while(1) {

        FD_ZERO(&readySet_);
        this->readySet_ = this->allSet_;

        if((numReady_ = select(NULL, &readySet_, NULL, NULL, NULL)) == SOCKET_ERROR) {
            continue;
        }

        if(FD_ISSET(listenSock_->getSock(), &readySet_)) {
            addSelectSock();
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
                nRead = find_sock(s).TCPRecv();
                if(nRead > 0) {
                    ProcessTCPPacket(find_sock(s).packet_);
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
        if(selectSocks_[i] < 0) {
            selectSocks_[i] = s;
            currentClients_.push_back(sock(s));
            break;
        }
    }
    FD_SET(s, &allSet_);
    return s;
}

QList<sock> tcpserver::getAllClients() {
    return currentClients_;
}

sock tcpserver::find_sock(SOCKET s) {
    for(int i = 0; i < currentClients_.size(); ++i) {
        if(currentClients_[i].getSock() == s) {
            return currentClients_[i];
        }
    }
    return sock();
}
