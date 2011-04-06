#ifndef FILETRANSFERIN_H
#define FILETRANSFERIN_H
#include <QThread>
#include <QString>
#include <winsock2.h>

class FileWriteThread : public QThread{
    Q_OBJECT

public:
    FileWriteThread(QString file);

protected:
    void run();
signals:
    void endFT();
    void displayMB(QString);
private:
    QString file_;
};

#endif // FILETRANSFERIN_H
