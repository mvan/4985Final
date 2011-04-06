#include <winsock2.h>
#include <QString>
#include "network.h"
#include "errors.h"
#include "externs.h"

void WinsockInit() {

    WORD version;
    WSADATA wsd;

    version = MAKEWORD(2,2);
    if(WSAStartup(version, &wsd) != 0) {
        WSAError(SOCK_ERROR);
    }

}

void WinsockCleanup() {
    WSACleanup();
}

//SERIOUSLY, THIS IS A SKETCHY FUNCTION, DON'T PASS ANY NULL PARAMETERS OR A BUFFER THAT ISN'T PACKETSIZE
void mkPacket(char* buf, char msgtype, unsigned short packetSize, char destClient, char* data) {
    memset(buf,0,PACKETSIZE);
    buf[0] = msgtype;
    buf[1] = LOBYTE(packetSize);
    buf[2] = HIBYTE(packetSize);
    buf[3] = destClient;
    memmove((buf+4), data, (int)packetSize);
}

unsigned short dataLength(char* buf) {
    return MAKEWORD((unsigned char)buf[1], (unsigned char)buf[2]);
}
