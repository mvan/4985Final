#ifndef CALLBACKS_H
#define CALLBACKS_H
#include <WinSock2.h>
LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK Serv_DlgProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK Cl_DlgProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);
void CALLBACK TCP_Comp_Routine(DWORD error, DWORD brx, LPWSAOVERLAPPED ol, DWORD flags);
void CALLBACK UDP_Comp_Routine(DWORD error, DWORD numTx, LPWSAOVERLAPPED ol, DWORD flags);
#endif