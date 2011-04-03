#include "filetransferin.h"
#include "buffer.h"
#include <QMessageBox>
#include <QFile>

Buffer fileinBuffer;
FileWriteThread::FileWriteThread(QString file):file_(file){}

void FileWriteThread::run(){

    char* packet;
    QMessageBox msg;

    QFile file(file_);

    if(!file.open(QIODevice::WriteOnly)){
        //error
    }

    msg.setText("File transfer complete");
    packet = (char *)malloc(PACKETSIZE);

    while(1){
        if(fileinBuffer.queue.size() == 0){
            fileinBuffer.queueMutex.lock();
            fileinBuffer.bufferNotEmpty.wait(&fileinBuffer.queueMutex);
            fileinBuffer.queueMutex.unlock();
        }
        fileinBuffer.grabPacket(packet);
         //If packet type is end of transmission, close handle, end thread
        if(packet[0] == MSG_FTCOMPLETE){
            file.close();
            free(packet);
            emit(endFT());
            msg.exec();
            return;
        }
         //length of packet
        //WriteFile(file_, (packet+4), dataLength(packet), &bytesWritten, NULL);
        file.write((packet+4), dataLength(packet));
    }
}
