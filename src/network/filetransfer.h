#ifndef FILETRANSFER_H
#define FILETRANSFER_H
#include <QThread>
#include <winsock2.h>

#define DATA_SIZE 4093

class FileReadThread : public QThread{
    Q_OBJECT

public:
    FileReadThread(HANDLE handle);

protected:
    void run();

private:
    HANDLE file_;
};

class FileSendThread : public QThread{
    Q_OBJECT

public:
    FileSendThread();

protected:
    void run();
};

#endif // FILETRANSFER_H
