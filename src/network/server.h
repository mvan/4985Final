#ifndef SERVER_H
#define SERVER_H
#include "network.h"
#include "errors.h"
#include "socket.h"
#include <QObject>
#include <QThread>
/**
  * Class: server
  *
  * Base class for a udp or tcp server.
  */
class server: public QThread {
    Q_OBJECT
    public :
        explicit server() {
            WinsockInit();
        }
        virtual ~server() {
            WinsockCleanup();
        }
        virtual void run(){}
};
#endif
