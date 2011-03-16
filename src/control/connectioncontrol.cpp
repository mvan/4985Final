#include "connectioncontrol.h"

ConnectionControl::ConnectionControl() {
}

ConnectionControl::~ConnectionControl() {
}

bool ConnectionControl::startServer(int port) {
    return true;
}

bool ConnectionControl::connectToServer(QString ip, int port) {
    //TODO: Duncan, we might need to be able to choose the port too.
    //Also, if you can return a boolean of success/failure, it would be nice.
    socket_.TCPSocket_Connect(ip.toAscii()); //<< if this fail return false to notify the gui
    return true;
}
