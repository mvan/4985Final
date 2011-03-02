#include <WinSock2.h>
#include "stats.h"
#include "resource.h"
#include "externs.h"
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: Serv_Stats
--
-- DATE: Feb 19, 2011
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Duncan Donaldson
--
-- PROGRAMMER: Duncan Donaldson
--
-- INTERFACE: void Serv_Stats()
--
--
-- RETURNS: void
--
-- NOTES:
-- updates the server stats portion of the server dialog window.
----------------------------------------------------------------------------------------------------------------------*/
void Serv_Stats() {

	int avgDelay,xferRate,avgSize,totalTime;
	static BOOL first = TRUE;

	//whether the program is currently connected.
	if (stats.STATE == STATCONNECT) {
		SetDlgItemText(hwndDlg, IDC_STATIC5, "CONNECTED");
	} else if (stats.STATE == STATDISCONNECT) {
		SetDlgItemText(hwndDlg, IDC_STATIC5, "DISCONNECTED");
	}

	//total UDP packets recvd
	SetDlgItemInt(hwndDlg, IDC_STATIC6, stats.TOTALUDPPACKETS, TRUE);

	//total TCP packets recvd
	SetDlgItemInt(hwndDlg, IDC_STATIC7, stats.TOTALTCPPACKETS, TRUE);

	if(stats.TOTALUDPPACKETS > 0) {
		//avg udp packet delay time.
		totalTime = GetTickCount()-stats.START_TIME;
		avgDelay = avgPacketTransferRate(totalTime, stats.TOTALDATA);
		SetDlgItemInt(hwndDlg, IDC_STATIC8, avgDelay, TRUE);
	} else {
		//avg TCP packet delay time.
		totalTime = GetTickCount()-stats.START_TIME;
		avgDelay = avgPacketTransferRate(totalTime, stats.TOTALTCPPACKETS);
		SetDlgItemInt(hwndDlg, IDC_STATIC8, avgDelay, TRUE);
	}

	//total transfer rate.
	xferRate = TransferRate(GetTickCount() - stats.START_TIME);
	SetDlgItemInt(hwndDlg, IDC_STATIC9, xferRate, TRUE);

	//average packetSize
	if(stats.TOTALTCPPACKETS == 0 && stats.TOTALUDPPACKETS == 0) {
		avgSize = 0;
	} else {
		avgSize = stats.TOTALDATA/(stats.TOTALTCPPACKETS+stats.TOTALUDPPACKETS);
	}
	SetDlgItemInt(hwndDlg, IDC_STATIC10, avgSize, TRUE);

	//total data received
	SetDlgItemInt(hwndDlg, IDC_STATIC11, stats.TOTALDATA, TRUE);
	return;

}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: Cli_Stats
--
-- DATE: Feb 19, 2011
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Duncan Donaldson
--
-- PROGRAMMER: Duncan Donaldson
--
-- INTERFACE: void Cli_Stats()
--
--
-- RETURNS: void
--
-- NOTES:
-- updates the client stats portion of the client dialog window.
----------------------------------------------------------------------------------------------------------------------*/
void Cli_Stats() {

	if (stats.STATE == STATCONNECT) {
		SetDlgItemText(hwndDlg, IDC_STATIC1, "CONNECTED");
	} else if (stats.STATE == STATDISCONNECT) {
		SetDlgItemText(hwndDlg, IDC_STATIC1, "DISCONNECTED");
	}

	//udp sent
	SetDlgItemInt(hwndDlg, IDC_STATIC2, stats.TOTALUDPPACKETS, TRUE);

	//tcp sent
	SetDlgItemInt(hwndDlg, IDC_STATIC3, stats.TOTALTCPPACKETS, TRUE);

	//total data sent
	SetDlgItemInt(hwndDlg, IDC_STATIC12, stats.TOTALDATA, TRUE);

}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: TransferRate
--
-- DATE: Feb 19, 2011
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Duncan Donaldson
--
-- PROGRAMMER: Duncan Donaldson
--
-- INTERFACE: int TransferRate(DWORD TotalTime, int NumPackets, int PacketSize)
--							- TotalTime		- the total time that the connection has been in progress.
--							- NumPackets	- the total number of packets transfered.
--							- PacketSize	- the size of each packet.
--
-- RETURNS: void
--
-- NOTES:
-- Calculates an approximate transfer rate based on total connection time, number of packets and packet size.
----------------------------------------------------------------------------------------------------------------------*/
int TransferRate(DWORD TotalTime) {
	if(TotalTime == 0) {
		return 0;
	}
	return ((stats.TOTALDATA / TotalTime)*1000);
}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: avgPacketTransferRate
--
-- DATE: Feb 19, 2011
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Duncan Donaldson
--
-- PROGRAMMER: Duncan Donaldson
--
-- INTERFACE: int avgPacketTransferRate(DWORD TotalTime, int NumPackets)
--							- TotalTime		- the total time that the connection has been in progress.
--							- NumPackets	- the total number of packets transfered.
--
-- RETURNS: int
--
-- NOTES:
-- Calculates an approximate average transfer time for packets.
----------------------------------------------------------------------------------------------------------------------*/
int avgPacketTransferRate(DWORD TotalTime, int NumPackets) {
	if(NumPackets == 0) {
		return 0;
	}
	return TotalTime / NumPackets;
}