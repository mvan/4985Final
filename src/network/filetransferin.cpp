#include "filetransferin.h"
#include "buffer.h"
#include <QMessageBox>
#include <QFile>

Buffer fileinBuffer;
FileWriteThread::FileWriteThread(QString file):file_(file){}

void FileWriteThread::run(){

    char packet[PACKETSIZE];

    QFile file(file_);

    if(!file.open(QIODevice::WriteOnly)){
        //error
    }

    while(1){
        ZeroMemory(packet, PACKETSIZE);
        if(fileinBuffer.queue.size() == 0){
            fileinBuffer.queueMutex.lock();
            fileinBuffer.bufferNotEmpty.wait(&fileinBuffer.queueMutex);
            fileinBuffer.queueMutex.unlock();
        }
        fileinBuffer.grabPacket(packet);
         //If packet type is end of transmission, close handle, end thread
        if(packet[0] == MSG_FTCOMPLETE){
            file.close();
            break;
        }
        file.write(packet+4, dataLength(packet));
    }
    emit(endFT());
}
