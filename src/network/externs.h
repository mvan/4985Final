#ifndef EXTERNS_H
#define EXTERNS_H
#include "buffer.h"
#include <QString>
#include "../application/appwindow.h"

extern Buffer fileinBuffer;
extern Buffer fileoutBuffer;
extern Buffer audioinBuffer;
extern Buffer audiooutBuffer;
extern Buffer chatinBuffer;
extern Buffer chatoutBuffer;
extern AppWindow *externAppWindow;
extern bool transferringOut;
extern bool streaming;
extern char ClientNum;
extern QString userName;

#endif // EXTERNS_H
