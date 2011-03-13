#include <winsock2.h>
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
