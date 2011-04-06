#include "audiotransfer.h"
#include "network.h"
#include "externs.h"
#include "errors.h"
#include "buffer.h"
#include <QFile>
Buffer audiooutBuffer;

AudioReadThread::AudioReadThread(QString file):file_(file){}

void AudioReadThread::run(){

    int sizeOfFile = 0;
    int bytesRead = 0;
    int totalRead = HDR_SIZE;
    char tempPacket[PACKETSIZE], tempBuf[DATA_SIZE], endPack[PACKETSIZE];
    char buf[HDR_SIZE];

    QFile file(file_);

    if(!file.open(QIODevice::ReadOnly)){
        WSAError(FILE_ERROR);
    }

    sizeOfFile = file.size();

    AudioSendThread *thread = new AudioSendThread();
    connect(thread, SIGNAL(sendPacket(char*)), this, SLOT(send(char*)), Qt::QueuedConnection);
    thread->start();

    file.read(buf, HDR_SIZE);

    while((totalRead <= sizeOfFile)){

        ZeroMemory(tempPacket, DATA_SIZE);
        ZeroMemory(tempBuf, DATA_SIZE);

        //More than a packet left
        if((sizeOfFile - totalRead) >= AUDIO_DATA_SIZE){

            memcpy(tempBuf, buf, HDR_SIZE);
            if((bytesRead = file.read(tempBuf+HDR_SIZE, AUDIO_DATA_SIZE)) == -1){
                WSAError(READ_ERROR);
            }
            mkPacket(tempPacket, MSG_AUDIO, (unsigned short)bytesRead, 0, tempBuf);
            if(audiooutBuffer.queue.size() == audiooutBuffer.bufferSize){
                audiooutBuffer.queueMutex.lock();
                audiooutBuffer.bufferNotFull.wait(&audiooutBuffer.queueMutex);
                audiooutBuffer.queueMutex.unlock();
            }
            audiooutBuffer.bufferPacket(tempPacket);
            totalRead += bytesRead;
        } else if(sizeOfFile == totalRead) { // finished exactly
            mkPacket(tempPacket, MSG_STREAMCOMPLETE, 0, 0, (char*)"");
            if(audiooutBuffer.queue.size() == audiooutBuffer.bufferSize){
                audiooutBuffer.queueMutex.lock();
                audiooutBuffer.bufferNotFull.wait(&audiooutBuffer.queueMutex);
                audiooutBuffer.queueMutex.unlock();
            }
            audiooutBuffer.bufferPacket(tempPacket);
            break;
        } else { //less than a full packet left
            memcpy(tempBuf, buf, HDR_SIZE);
            if((bytesRead = file.read((tempBuf+HDR_SIZE), (sizeOfFile - totalRead))) == -1){
                WSAError(READ_ERROR);
            }
            mkPacket(tempPacket, MSG_AUDIO, (unsigned short)bytesRead, 0, tempBuf);
            if(audiooutBuffer.queue.size() == audiooutBuffer.bufferSize){
                audiooutBuffer.queueMutex.lock();
                audiooutBuffer.bufferNotFull.wait(&audiooutBuffer.queueMutex);
                audiooutBuffer.queueMutex.unlock();
            }
            audiooutBuffer.bufferPacket(tempPacket);
            //make EOT packet
            memset(tempBuf, 0, sizeof(tempBuf));
            mkPacket(endPack, MSG_STREAMCOMPLETE, 0, 0,(char*)"");
            if(audiooutBuffer.queue.size() == audiooutBuffer.bufferSize){
                audiooutBuffer.queueMutex.lock();
                audiooutBuffer.bufferNotFull.wait(&audiooutBuffer.queueMutex);
                audiooutBuffer.queueMutex.unlock();
            }
            audiooutBuffer.bufferPacket(tempPacket);
            break;
        }
        thread->wait(10);
    }
    thread->wait();

    file.close();
    disconnect(thread, SIGNAL(sendPacket(char*)), this, SLOT(send(char*)));
    streamingOut = false;
    emit(endStream());
    return;
}

void AudioReadThread::send(char* packet){
    emit sendUDPPacket(packet);
}

AudioSendThread::AudioSendThread(){}

void AudioSendThread::run(){

    char packet[PACKETSIZE];

    while(1){

        ZeroMemory(packet, PACKETSIZE);
        if(audiooutBuffer.queue.empty()){
            audiooutBuffer.queueMutex.lock();
            audiooutBuffer.bufferNotEmpty.wait(&audiooutBuffer.queueMutex);
            audiooutBuffer.queueMutex.unlock();
        }

        audiooutBuffer.grabPacket(packet);

        emit sendPacket(packet);
        Sleep(1);
        if(packet[0] == MSG_STREAMCOMPLETE || packet[0] == MSG_MICCLOSED){
            break;
        }

    }
    Sleep(10);
}
