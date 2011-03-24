#include "audiotransferin.h"
#include "buffer.h"
#include <QMessageBox>

Buffer audioinBuffer;

AudioWriteThread::AudioWriteThread(QByteArray array):audioArray_(array){

}

void AudioWriteThread::run(){

    char* packet;
    QMessageBox msg;
    msg.setText("File transfer complete");
    packet = (char *)malloc(PACKETSIZE * sizeof(char *));

    while(1){
        audioinBuffer.queueMutex.lock();
        if(audioinBuffer.queue.size() != 0){

            packet = audioinBuffer.grabPacket();
            audioinBuffer.queueMutex.unlock();
            if(packet[0] == MSG_FTCOMPLETE){   //If packet type is end of transmission, close handle, end thread
                //CloseHandle(file_);
                msg.exec();
                return;
            }
            //WriteFile(file_, packet, DATA_SIZE, &bytesWritten, NULL); //length of packet
            audioArray_.append(packet+1);
        }
        audioinBuffer.queueMutex.unlock();
    }
}

