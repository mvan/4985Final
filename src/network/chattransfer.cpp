#include "chattransfer.h"
#include "network.h"
#include "buffer.h"

Buffer chatoutBuffer;
void ChatSendThread::run(){

    QMutex mutex;
    DWORD bytesWritten;
    char* packet;

   while(1){
        chatoutBuffer.queueMutex.lock();
        if(chatoutBuffer.queue.size() != 0){
            packet = chatoutBuffer.grabPacket();
            //Send fileoutBuffer.grabPacket()
            /*if(packet[0] == MSG_FTCOMPLETE){
                chatoutBuffer.queueMutex.unlock();
                break;
            }*/
        }
        chatoutBuffer.queueMutex.unlock();
    }
}
