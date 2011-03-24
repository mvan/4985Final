#ifndef SERVERCONTROL_H
#define SERVERCONTROL_H

#include <QList>

#include "../global.h"
#include "../network/socket.h"

class ServerControl {
public:
    ServerControl();
    ~ServerControl();

    bool addAudioFile(QString filename);

private:
};

#endif // SERVERCONTROL_H
