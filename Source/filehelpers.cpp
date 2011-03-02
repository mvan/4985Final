#include <crtdbg.h>
#include <WinSock2.h>
#include "helpers.h"
#include "network.h"
#include "buffer.h"
#include "externs.h"
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: GetFileName
--
-- DATE: Nov 29, 2010
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Duncan Donaldson
--
-- PROGRAMMER: Duncan Donaldson
--
-- INTERFACE: void GetFileName(HWND hwnd, LPSTR FileName, LPSTR FileTitle)
--							- hwnd		- the handle to the program's main window.
--							- FileName  - the Full path of the file selected will be assigned to this.
--							- FileTitle	- the name of the file selected will be assigned to this.
--
--
-- RETURNS: the return value of GetOpenFileName.
--
-- NOTES:
-- Opens a FileOpenDialog, allowing the user to select a file name, then assigns the file's name and path to
-- the FileTitle and FileName variables.
----------------------------------------------------------------------------------------------------------------------*/
int GetFileName (HWND hwnd, LPSTR FileName, LPSTR FileTitle) {
     OPENFILENAME ofn;

     static TCHAR* szFilter = "All Files (*.*)\0*.*\0\0";
     ZeroMemory(&ofn, sizeof(ofn));

     ofn.lStructSize       = sizeof(OPENFILENAME);
     ofn.lpstrFilter       = szFilter;
     ofn.nMaxFile          = MAX_PATH;
     ofn.nMaxFileTitle     = MAX_PATH;
     ofn.lpstrDefExt       = TEXT("");
     ofn.hwndOwner         = hwnd;
     ofn.lpstrFile         = FileName;
     ofn.lpstrFileTitle    = FileTitle;
     ofn.Flags             = OFN_CREATEPROMPT | OFN_FILEMUSTEXIST | OFN_EXPLORER;
     
     return GetOpenFileName (&ofn);
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: FileWrite
--
-- DATE: Nov 29, 2010
--
-- REVISIONS: Feb 19th, 2011 - changed to work with TCP/UDP assignment
--
-- DESIGNER: Duncan Donaldson
--
-- PROGRAMMER: Duncan Donaldson
--
-- INTERFACE: DWORD FileWrite(HANDLE file, char* data, DWORD size)
--							- file			- handle to the file to write to.
--							- data			- the block of data to be written.
--							- size			- the size of the block of data to be written.
--
--
-- RETURNS: the number of bytes written to file.
--
-- NOTES:
-- writes a block of data to a file.
----------------------------------------------------------------------------------------------------------------------*/
DWORD FileWrite(HANDLE hFile, char* data, DWORD size) {
	DWORD bytesWritten;

	WriteFile(hFile, data, size, &bytesWritten, NULL);

	return bytesWritten;
}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: BufferFile
--
-- DATE: Nov 29, 2010
--
-- REVISIONS: Feb 19, 2011 - Changed to work with TCP/UDP assignment
--
-- DESIGNER: Duncan Donaldson
--
-- PROGRAMMER: Duncan Donaldson
--
-- INTERFACE: DWORD WINAPI BufferFile(LPVOID pSize)
--							- pSize - empty paramter.
--
--
-- RETURNS: DWORD - returns 0 if something breaks, 1 otherwise.
--
-- NOTES:
-- Reads all the data from a given file and stores it in blocks the size of the packet, as specified by the user.
----------------------------------------------------------------------------------------------------------------------*/
DWORD BufferFile(LPVOID pSize) {

	int bytesToRead = packetSize;
	LONGLONG fileSize;
	LARGE_INTEGER l;
	DWORD bytesRead;
	char* data;

	numPackets = 0;

	if((file = CreateFile((LPCSTR)FilePath,GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0)) == INVALID_HANDLE_VALUE) {
		return 0;
	} else {

		GetFileSizeEx(file,&l);
		fileSize = l.QuadPart;

		do {

			data = (char*)malloc(packetSize);
			if(data == NULL) {
				return 0;
			}

			bytesToRead = (fileSize > packetSize)?packetSize:(fileSize);
			fileSize -= bytesToRead;
			ReadFile(file, data, bytesToRead, &bytesRead, 0);
			EnterCriticalSection(&critsect);
			bufferData(data, bytesToRead);
			LeaveCriticalSection(&critsect);
			numPackets++;
			free(data);

		} while(fileSize > 0);

	}
	CloseHandle(file);
	return 1;
}