#include "micthread.h"
#include "audioin.h"
#include "../network/audiotransfer.h"
bool micOut = FALSE;
MicThread::MicThread(){}

void MicThread::run(){

    AudioSendThread *thread;
    audioin in;

    thread = new AudioSendThread();

    connect(thread, SIGNAL(sendPacket(char*)), this, SLOT(send(char*)), Qt::QueuedConnection);
    in.setupParams();
    in.createAudioDev();

    thread->start();

    while(micOut){
        in.readSound();
    }
    thread->wait();
    disconnect(thread, SIGNAL(sendPacket(char*)), this, SLOT(send(char*)));
    emit(endMic());
    return;
}

void MicThread::send(char* packet){
    emit sendUDPPacket(packet);
}
