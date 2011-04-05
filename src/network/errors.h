#ifndef ERRORS_H
#define ERRORS_H

#include <winsock2.h>

#define SOCK_ERROR 1
#define RD_ERROR 2
#define WR_ERROR 3
#define EVT_ERROR 4
#define THREAD_ERROR 5
#define ALLOC_ERROR 6
#define FILE_ERROR 7
#define READ_ERROR 8
void WSAError(int error);
#endif
