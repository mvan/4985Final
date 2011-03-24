  #include <winsock2.h>
#include <QString>
#include "network.h"
#include "errors.h"
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
    ZeroMemory(buf, PACKETSIZE);
    buf[0] = msgtype;
    buf[1] = LOBYTE(packetSize);
    buf[2] = HIBYTE(packetSize);
    buf[3] = destClient;
    memmove((buf+4), data, PACKETSIZE-4);
}

void ProcessUDPPacket(char* packet) {
    switch(packet[0]) {
        default:
            break;
            //push data on to stream buffer.
    }
}

void ProcessTCPPacket(char* packet) {
    switch(packet[0]) {
        case MSG_FT:
            //send file data.
            break;
        case MSG_FTCOMPLETE:
            //send message to close file
            break;
        case MSG_CHAT:
            //display to chat window
            break;
        case MSG_LIST:
            break;
            //send list of all songs on playlist.
        default:
            break;
    }
}

unsigned short dataLength(char* buf) {
    return MAKEWORD(buf[1], buf[2]);
}
