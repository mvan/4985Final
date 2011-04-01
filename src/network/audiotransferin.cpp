#include "audiotransferin.h"
#include "buffer.h"
#include <QMessageBox>

Buffer audioinBuffer;

AudioWriteThread::AudioWriteThread(QByteArray array):audioArray_(array){

}

void AudioWriteThread::run(){

    QMutex mutex;
    char* packet;
    QMessageBox msg;
    msg.setText("File transfer complete");
    packet = (char *)malloc(PACKETSIZE);

    while(1){
        if(audioinBuffer.queue.size() == 0){
            audioinBuffer.queueMutex.lock();
            audioinBuffer.bufferNotEmpty.wait(&audioinBuffer.queueMutex);
            audioinBuffer.queueMutex.unlock();
        }
        audioinBuffer.grabPacket(packet);
        //If packet type is end of transmission, end thread
        if(packet[0] == MSG_STREAMCOMPLETE){
            msg.exec();
            free(packet);
            emit(endStream());
            return;
        }
        audioArray_.append(packet+1);


    }
}

