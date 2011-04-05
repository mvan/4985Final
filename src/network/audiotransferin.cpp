#include "audiotransferin.h"
#include "buffer.h"
#include "../audio/audioout.h"
#include <QMessageBox>

Buffer audioinBuffer;

AudioWriteThread::AudioWriteThread(){

}

void AudioWriteThread::run(){

    char packet[PACKETSIZE];
    audioout out;

    while(1){
        if(audioinBuffer.queue.size() == 0){
            audioinBuffer.queueMutex.lock();
            audioinBuffer.bufferNotEmpty.wait(&audioinBuffer.queueMutex);
            audioinBuffer.queueMutex.unlock();
        }
        audioinBuffer.grabPacket(packet);
        if(out.getParams(packet) == 1){
            out.setupParams();
            out.createAudioDev();
        }

        //If packet type is end of transmission, end thread
        if(packet[0] == MSG_STREAMCOMPLETE){
            break;
        }
        out.playSound(packet);
    }
    Sleep(10);
    emit(endStream());
}

