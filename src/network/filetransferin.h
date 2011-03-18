#ifndef FILETRANSFERIN_H
#define FILETRANSFERIN_H
#include <QThread>
#include <winsock2.h>

class FileWriteThread : public QThread{
    Q_OBJECT

public:
    FileWriteThread(HANDLE handle);

protected:
    void run();

private:
    HANDLE file_;
};

#endif // FILETRANSFERIN_H
