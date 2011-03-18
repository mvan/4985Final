#ifndef FILETRANSFER_H
#define FILETRANSFER_H
#include <QThread>
#include <winsock2.h>

#define DATA_SIZE 4093

class FileTransferThread : public QThread{
    Q_OBJECT

public:
    FileTransferThread(HANDLE handle);

protected:
    void run();

private:
    HANDLE file_;
};

#endif // FILETRANSFER_H
