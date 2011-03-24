#include "chattransferin.h"
#include "buffer.h"
#include <QMessageBox>

Buffer chatinBuffer;


void ChatWriteThread::run(){

    char* packet;
    packet = (char *)malloc(PACKETSIZE * sizeof(char *));

    while(1){
        chatinBuffer.queueMutex.lock();
        if(chatinBuffer.queue.size() != 0){

            packet = chatinBuffer.grabPacket();
            chatinBuffer.queueMutex.unlock();
            //add packet data to chat display
        }
        chatinBuffer.queueMutex.unlock();
    }
}


