#include "micthread.h"
#include "audioin.h"
#include "../network/audiotransfer.h"
bool micOut = FALSE;
MicThread::MicThread(){}

void MicThread::run(){

    AudioSendThread *thread;
    audioin in;

    //connect(thread, SIGNAL(sendPacket(char*)), this, SLOT(send(char*)), Qt::QueuedConnection);
    in.setupParams();
    in.createAudioDev();

    thread->start();

    while(micOut){
        in.readSound();
    }

}

/*void MicThread::send(char*){

}*/
