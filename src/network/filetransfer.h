#ifndef FILETRANSFER_H
#define FILETRANSFER_H
#include <QThread>
#include <winsock2.h>



class FileReadThread : public QThread{
    Q_OBJECT

public:
    FileReadThread(QString file);

protected:
    void run();

private:
    QString file_;
};

class FileSendThread : public QThread{
    Q_OBJECT

public:
    FileSendThread();

protected:
    void run();
};

#endif // FILETRANSFER_H
