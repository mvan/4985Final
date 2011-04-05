#include "micthread.h"
#include "audioin.h"
bool micOut = FALSE;
MicThread::MicThread(){}

void MicThread::run(){

    audioin in;
    in.setupParams();
    in.createAudioDev();

    while(micOut){
        in.readSound();
    }

}
