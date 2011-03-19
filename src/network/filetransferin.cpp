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
        mutex.lock();
        if(fileinBuffer.queue.size() != 0){
            mutex.unlock();
            packet = fileinBuffer.grabPacket();
            WriteFile(file_, packet, DATA_SIZE, &bytesWritten, NULL);
            /*if(packet[3] == EOT){   If packet type is end of transmission, close handle, end thread
                CloseHandle(file_);
                msg.exec();
                return;
            }*/
        }
        mutex.unlock();
    }
}
