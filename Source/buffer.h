#ifndef BUFFER_H
#define BUFFER_H
typedef struct data_buf {
	char* buf;
	int len;
	data_buf* next;
} BUFFER, *PBUFFER;
void bufferData(char* data, int size);
DWORD WINAPI WriteBufToFile(LPVOID fPath);
int popTail(char* buf, int size);
#endif