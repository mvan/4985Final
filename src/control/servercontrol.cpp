#include "servercontrol.h"

ServerControl::ServerControl(ConnectionControl *connectionControl):
    connectionControl_(connectionControl){
    socket_ = connectionControl->getSocket();
}

ServerControl::~ServerControl() {
}

bool ServerControl::addAudioFile(QString filename) {
    PlaylistItem item;
    item.filename = filename;
    void *test = &item;
    size_t sizetest =sizeof(item) + item.filename.size();
    memcpy(socket_->packet_, (unsigned char *)&item, sizeof(item) + item.filename.size());
    socket_->TCPSend();
    return true;
}
