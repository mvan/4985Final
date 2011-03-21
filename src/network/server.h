#ifndef SERVER_H
#define SERVER_H
#include "network.h"
#include "errors.h"
#include "socket.h"
class server {
    public :
        explicit server() {
            WinsockInit();
        }
        virtual ~server() {
            WinsockCleanup();
        }
        virtual void run(int portNo) = 0;
};
#endif
