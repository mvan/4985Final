#ifndef NETWORK_H
#define NETWORK_H
/* Defines for message types*/
#define MULTICAST_ADDR "234.98.98.98"
#define MULTICAST_TTL 2
#define TCPPORT 7898
#define UDPPORT 7899
#define PACKETSIZE 4096
#define MSG_FTREQ 0x01
#define MSG_FT 0x02
#define MSG_FTCOMPLETE 0x03
#define MSG_STREAMREQ 0x04
#define MSG_STREAMPAUSE 0x05
#define MSG_STREAMCOMPLETE 0x06
#define MSG_AUDIO 0x07
#define MSG_CHAT 0x08
#define MSG_LISTREQ 0x09
#define MSG_LIST 0x0A
#define MSG_ACK  0x0B
#define MSG_CONN 0x0C
#define MSG_CONNBACK 0x0D
#define MSG_MIC 0x0E
#define MSG_MICOPEN 0x0F
#define MSG_MICCLOSED 0x10
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
void WinsockInit();
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
void WinsockCleanup();
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: mkPacket
--
-- DATE: Feb 19, 2011
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Duncan Donaldson
--
-- PROGRAMMER: Duncan Donaldson
--
-- INTERFACE: void mkPacket(char* buf, char msgtype, unsigned short packetSize,
--                                        char destClient, char* data
--
--                  buf - Buffer to write packet into
--                  msgtype - Type of message
--                  packetSize - Total length of header and data
--                  destClient - ID number of destination client
--                  data - Data to be written to packet
-- RETURNS: void
--
-- NOTES:
-- Constructs a packet with the given message type and data section.
----------------------------------------------------------------------------------------------------------------------*/
void mkPacket(char* buf, char msgtype, unsigned short packetSize,
                                        char destClient, char* data);
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: dataLength(char* buf)
--
-- DATE: Feb 19, 2011
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Duncan Donaldson
--
-- PROGRAMMER: Duncan Donaldson
--
-- INTERFACE: void dataLength(char* buf)
--
--                  buf - packet to determine length of
--
-- RETURNS: unsigned short
--
-- NOTES:
-- Constructs the length of the supplied packet
----------------------------------------------------------------------------------------------------------------------*/
unsigned short dataLength(char* buf);
#endif
