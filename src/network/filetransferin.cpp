#include "filetransferin.h"
#include "buffer.h"
#include <QMessageBox>

Buffer fileinBuffer;
FileWriteThread::FileWriteThread(HANDLE handle):file_(handle){

}

void FileWriteThread::run(){

    char* packet;
    QMutex mutex;
    DWORD bytesWritten;
    QMessageBox msg;
    msg.setText("File transfer complete");
    packet = (char *)malloc(PACKETSIZE * sizeof(char *));

    while(1){
        if(fileinBuffer.queue.size() == 0){
            fileinBuffer.queueMutex.lock();
            fileinBuffer.bufferNotEmpty.wait(&fileinBuffer.queueMutex);
            fileinBuffer.queueMutex.unlock();
        }
        fileinBuffer.grabPacket(packet);
        if(packet[0] == MSG_FTCOMPLETE){   //If packet type is end of transmission, close handle, end thread
            CloseHandle(file_);
            msg.exec();
            free(packet);
            return;
        }
        WriteFile(file_, (packet+4), dataLength(packet), &bytesWritten, NULL); //length of packet

    }
}
