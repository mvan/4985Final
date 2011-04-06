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
extern QString userName;
extern bool transferring;
extern bool streamingOut;
extern char ClientNum;

#endif // EXTERNS_H
