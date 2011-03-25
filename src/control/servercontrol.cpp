#include "servercontrol.h"

ServerControl::ServerControl(ConnectionControl *connectionControl):
    connectionControl_(connectionControl){
}

ServerControl::~ServerControl() {
}

bool ServerControl::addAudioFile(QString filename) {
    clientsSocket_ = connectionControl_->getTCPServerThread()->getTCPServer()->getAllClients();
    foreach(sock socket, clientsSocket_) {
        mkPacket(socket.packet_, MSG_LIST, filename.size(), 0x00, filename.toAscii().data()); //Change the client destination
        socket.TCPSend();
    }
    return true;
}
