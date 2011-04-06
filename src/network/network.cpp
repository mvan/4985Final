#include <winsock2.h>
#include <QString>
#include "network.h"
#include "errors.h"
#include "externs.h"
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: WinsockInit
--
-- DATE: Feb 19, 2011
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Duncan Donaldson
--
-- PROGRAMMER: Duncan Donaldson
--
-- INTERFACE: void WinsockInit()
--
-- RETURNS: void
--
-- NOTES:
-- Initializes a Winsock session.
----------------------------------------------------------------------------------------------------------------------*/
void WinsockInit() {

    WORD version;
    WSADATA wsd;

    version = MAKEWORD(2,2);
    if(WSAStartup(version, &wsd) != 0) {
        WSAError(SOCK_ERROR);
    }

}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: WinsockCleanup
--
-- DATE: Feb 19, 2011
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Duncan Donaldson
--
-- PROGRAMMER: Duncan Donaldson
--
-- INTERFACE: void WinsockCleanup(socket)
--
-- RETURNS: void
--
-- NOTES:
-- cleans up a winsock session.
----------------------------------------------------------------------------------------------------------------------*/
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
    memcpy((buf+4), data, (int)packetSize);
}

void ProcessUDPPacket(char* packet) {
    if(packet[0] == MSG_AUDIO) {
        audioinBuffer.bufferPacket(packet);
    }
}

unsigned short dataLength(char* buf) {
    return MAKEWORD((unsigned char)buf[1], (unsigned char)buf[2]);
}
