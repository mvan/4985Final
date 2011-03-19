#ifndef CONNECTIONCONTROL_H
#define CONNECTIONCONTROL_H

#include <QString>

#include "../network/socket.h"

class ConnectionControl {
public:
    ConnectionControl();
    ~ConnectionControl();

    bool startServer(int port);
    bool connectToServer(QString ip, int port);

private:
    struct sock socket_;
};

#endif // CONNECTIONCONTROL_H
