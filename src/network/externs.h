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
extern bool transferringOut;
extern bool transferringIn;
extern bool streamingOut;
extern bool streamingIn;
extern bool micOut;
extern char ClientNum;

#endif // EXTERNS_H
