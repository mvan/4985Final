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
signals:
    void endFT();
private:
    HANDLE file_;
};

#endif // FILETRANSFERIN_H
