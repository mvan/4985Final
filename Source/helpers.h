#ifndef HELPERS_H
#define HELPERS_H
#include<WinSock2.h>
int GetFileName (HWND hwnd, LPSTR FileName, LPSTR FileTitle);
DWORD FileWrite(HANDLE hFile, char* data, DWORD size);
DWORD BufferFile(LPVOID pSize);
#endif