#include <crtdbg.h>
#include <stdlib.h>
#include <WinSock2.h>
#include "buffer.h"
#include "network.h"
#include "helpers.h"
#include "externs.h"
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: bufferData
--
-- DATE: Feb 22, 2011
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Duncan Donaldson
--
-- PROGRAMMER: Duncan Donaldson
--
-- INTERFACE: void bufferData(char* data, int size)
--							data - the string to be buffered.
--							size - the size of the string to be buffered.
--
--
-- RETURNS: void
--
-- NOTES:
-- buffers a string in a dynamically allocated struct.
----------------------------------------------------------------------------------------------------------------------*/
void bufferData(char* data, int size) {

	PBUFFER newbuf;
	char* tmp;
	
	tmp = (char*)malloc(size);
	if(tmp == NULL) {
		WSAError(ALLOC_ERROR);
	}

	memmove(tmp, data, size);

	if(tail == NULL) {

		tail = (PBUFFER)malloc(sizeof(BUFFER));
		if(tail == NULL) {
			WSAError(ALLOC_ERROR);
		}

		tail->buf = tmp;
		tail->len = size;

		tail->next = NULL;
		numBuffers++;
		return;

	} else if(head == NULL) {

		head = (PBUFFER)malloc(sizeof(BUFFER));
		if(head == NULL) {
			WSAError(ALLOC_ERROR);
		}

		head->buf = tmp;
		head->len = size;

		tail->next = head;
		head->next = tail;
		numBuffers++;
		return;

	} else {

		newbuf = (PBUFFER)malloc(sizeof(BUFFER));
		if(newbuf == NULL) {
			WSAError(ALLOC_ERROR);
		}

		newbuf->buf = tmp;
		newbuf->len = size;

		newbuf->next = head;
		head=newbuf;
		tail->next = newbuf;
		numBuffers++;

	}
}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: WriteBufToFile
--
-- DATE: Feb 22, 2011
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Duncan Donaldson
--
-- PROGRAMMER: Duncan Donaldson
--
-- INTERFACE: DWORD WINAPI WriteBufToFile(LPVOID fpath)
--							fpath - arbitrary argument
--
--
-- RETURNS: arbitrary return value.
--
-- NOTES:
-- writes the contents of the global data buffer to a file.
----------------------------------------------------------------------------------------------------------------------*/
DWORD WINAPI WriteBufToFile(LPVOID fPath) {

	HANDLE file;
	char* buf;
	int size = 0;

	if(!(file = CreateFile(FilePath, FILE_APPEND_DATA, FILE_SHARE_WRITE,
							NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL))) {
		WSAError(FILE_ERROR);
	}

	while(1) {

		if(numBuffers == 0) {
			Sleep(1000);
		} else {
			size = tail->len;
			buf = (char*)malloc(size);
			if(buf == NULL) {
				WSAError(ALLOC_ERROR);
			}
			EnterCriticalSection(&critsect);
			if(popTail(buf, size) != 0) {
				FileWrite(file, buf, size);
			}
			free(buf);
			LeaveCriticalSection(&critsect);
		}

	}

	CloseHandle(file);
}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: popTail
--
-- DATE: Feb 22, 2011
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Duncan Donaldson
--
-- PROGRAMMER: Duncan Donaldson
--
-- INTERFACE: int bufferData(char* buf, int size)
--							buf  - the string to be filled with the data of the tail buffer.
--							size - the size of the string to be copied.
--
--
-- RETURNS: int
--
-- NOTES:
-- pops the end off the global data buffer and writes its data to buf.
----------------------------------------------------------------------------------------------------------------------*/
int popTail(char* buf, int size) {
	PBUFFER tmp = head;

	if(tail == NULL) {
		return 0;
	}
	if(numBuffers == 0) {
		return 0;
	}
	if(numBuffers == 1) {
		memmove(buf, tail->buf, size);
		free(tail->buf);
		free(tail);
		numBuffers--;
		return 1;
	}

	while(tmp->next != head) {
		if(tmp->next == tail) {
			memmove(buf, tail->buf, size);
			free(tail->buf);
			free(tail);
			tail = tmp;
			tail->next = head;
			numBuffers--;
			return 1;
		}
		tmp = tmp->next;
	}

	return 0;
}