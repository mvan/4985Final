#ifndef SERVERCONTROL_H
#define SERVERCONTROL_H

#include <QList>

#include "../global.h"
#include "../network/socket.h"
#include "connectioncontrol.h"

class ServerControl {
public:
    ServerControl(ConnectionControl * connectionControl);
    ~ServerControl();

    bool addAudioFile(QString filename);

private:
    ConnectionControl* connectionControl_;
    sock* socket_;
};

#endif // SERVERCONTROL_H
