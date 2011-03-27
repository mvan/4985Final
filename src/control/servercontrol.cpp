#include "servercontrol.h"

ServerControl::ServerControl(ConnectionControl *connectionControl):
    connectionControl_(connectionControl){
    socket_ = connectionControl->getTCPSocket();
}

ServerControl::~ServerControl() {
}

bool ServerControl::addAudioFile(QString filename) {
    //memcpy(socket_->packet_, (unsigned char *)&item, sizeof(item) + item.filename.size());
    socket_->TCPSend();
    return true;
}
