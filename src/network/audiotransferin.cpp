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

        ZeroMemory(packet, PACKETSIZE);
        if(audioinBuffer.queue.size() == 0){
            audioinBuffer.queueMutex.lock();
            audioinBuffer.bufferNotEmpty.wait(&audioinBuffer.queueMutex);
            audioinBuffer.queueMutex.unlock();
        }
        audioinBuffer.grabPacket(packet);
        if(out.getParams((packet+4)) == 1){
            out.destroyAudioDev();
            out.setupParams();
            out.createAudioDev();
        }
        out.playSound(packet);
    }
}

