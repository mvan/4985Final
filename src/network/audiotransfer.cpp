#include "audiotransfer.h"
#include "network.h"
#include "buffer.h"
#include <QFile>

Buffer audiooutBuffer;

AudioReadThread::AudioReadThread(QString file):file_(file){}

void AudioReadThread::run(){

    int sizeOfFile = 0;
    int numOfReads = 0;
    int bytesRead = 0;
    char* tempPacket;
    char* tempBuf;

    tempPacket = (char *)malloc(PACKETSIZE);
    tempBuf = (char *)malloc(DATA_SIZE);
    ZeroMemory(tempPacket, PACKETSIZE);
    ZeroMemory(tempBuf, DATA_SIZE);

    QFile file(file_);

    if(!file.open(QIODevice::ReadOnly)){
        //error
    }

    sizeOfFile = file.size();


    AudioSendThread *thread = new AudioSendThread();
    thread->start();

    while((numOfReads * DATA_SIZE) < sizeOfFile){
        if((sizeOfFile - (numOfReads * DATA_SIZE)) > DATA_SIZE){ //More than a packet left
            if((bytesRead = file.read(tempBuf, DATA_SIZE)) == -1){
                //error reading file
            }
            mkPacket(tempPacket, MSG_AUDIO, (unsigned short)bytesRead, 0,tempBuf); //change with src and dest, msg type
            if(audiooutBuffer.queue.size() == audiooutBuffer.bufferSize){
                audiooutBuffer.queueMutex.lock();
                audiooutBuffer.bufferNotFull.wait(&audiooutBuffer.queueMutex);
                audiooutBuffer.queueMutex.unlock();
            }
            audiooutBuffer.bufferPacket(tempPacket);
            ++numOfReads;
        } else if((sizeOfFile - (numOfReads * DATA_SIZE)) == 0) { // finished exactly
            memset(tempBuf, 0, sizeof(tempBuf));
            mkPacket(tempPacket, MSG_FTCOMPLETE, 0, 0, tempBuf);
            if(audiooutBuffer.queue.size() == audiooutBuffer.bufferSize){
                audiooutBuffer.queueMutex.lock();
                audiooutBuffer.bufferNotFull.wait(&audiooutBuffer.queueMutex);
                audiooutBuffer.queueMutex.unlock();
            }
            audiooutBuffer.bufferPacket(tempPacket);
            file.close();
            free(tempPacket);
            free(tempBuf);
            break;
        } else { //less than a full packet left
            if((bytesRead = file.read(tempBuf, sizeOfFile - (numOfReads * DATA_SIZE))) == -1){
                //error
            }
            mkPacket(tempPacket, MSG_AUDIO, (unsigned short)bytesRead, 0, tempBuf); //change with src and dest, msg type
            if(audiooutBuffer.queue.size() == audiooutBuffer.bufferSize){
                audiooutBuffer.queueMutex.lock();
                audiooutBuffer.bufferNotFull.wait(&audiooutBuffer.queueMutex);
                audiooutBuffer.queueMutex.unlock();
            }
            audiooutBuffer.bufferPacket(tempPacket);
            //make EOT packet
            memset(tempBuf, 0, sizeof(tempBuf));
            mkPacket(tempPacket, MSG_FTCOMPLETE, 0, 0, tempBuf);
            if(audiooutBuffer.queue.size() == audiooutBuffer.bufferSize){
                audiooutBuffer.queueMutex.lock();
                audiooutBuffer.bufferNotFull.wait(&audiooutBuffer.queueMutex);
                audiooutBuffer.queueMutex.unlock();
            }
            audiooutBuffer.bufferPacket(tempPacket);

            file.close();
            free(tempPacket);
            free(tempBuf);
            emit(endStream());
            break;
        }
        thread->wait(10);
    }
    thread->wait();
}

AudioSendThread::AudioSendThread(){}

void AudioSendThread::run(){

    char* packet;
    packet = (char *)malloc(PACKETSIZE);

    while(1){
        if(audiooutBuffer.queue.size() == 0){
            audiooutBuffer.queueMutex.lock();
            audiooutBuffer.bufferNotEmpty.wait(&audiooutBuffer.queueMutex);
            audiooutBuffer.queueMutex.unlock();
        }
        audiooutBuffer.grabPacket(packet);
        //Send fileoutBuffer.grabPacket()
        if(packet[0] == MSG_FTCOMPLETE){
            free(packet);
            break;
        }

    }
}
