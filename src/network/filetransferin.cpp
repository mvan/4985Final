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
        fileinBuffer.queueMutex.lock();
        if(fileinBuffer.queue.size() != 0){

            packet = fileinBuffer.grabPacket();
            fileinBuffer.queueMutex.unlock();
            if(packet[0] == MSG_FTCOMPLETE){   //If packet type is end of transmission, close handle, end thread
                CloseHandle(file_);
                msg.exec();
                return;
            }
            WriteFile(file_, (packet+1), PACKETSIZE - 1, &bytesWritten, NULL); //length of packet

        }
        fileinBuffer.queueMutex.unlock();
    }
}
