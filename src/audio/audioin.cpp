#include "audioin.h"
#include "../network/externs.h"
#include "../network/network.h"
void audioin::setupParams() {
    format_.setFrequency(44100);
    format_.setChannels(2);
    format_.setSampleSize(16);
    format_.setCodec("audio/pcm");
    format_.setByteOrder(QAudioFormat::LittleEndian);
    format_.setSampleType(QAudioFormat::SignedInt);
}

void audioin::createAudioDev() {
    QAudioDeviceInfo i(QAudioDeviceInfo::defaultInputDevice());
    if(!i.isFormatSupported(format_)) {
        format_ = i.nearestFormat(format_);
    } else {
        input_ = new QAudioInput(format_);
        buffer_ = new QBuffer();
        buffer_->open(QIODevice::ReadWrite);
        input_->start(buffer_);
        inbuf_ = (QBuffer*)buffer_;
        connect(inbuf_, SIGNAL(bytesWritten(qint64)), this, SLOT(writeReady(qint64)));
        connect(inbuf_, SIGNAL(readyRead()), this, SLOT(readSound()));
    }
}

void audioin::destroyAudioDev() {
    delete input_;
    delete buffer_;
}

void audioin::readSound(){

    char buf[DATA_SIZE];
    char packet[PACKETSIZE];
    qint64 availableData = numToRead_;
    ZeroMemory(buf, DATA_SIZE);
    ZeroMemory(packet, PACKETSIZE);

    mkVoiceHdr(buf);
    inbuf_->seek(inbuf_->size() - numToRead_);
    while(availableData > 0) {
        if(availableData >= AUDIO_DATA_SIZE) {
            inbuf_->read(buf+HDR_SIZE, AUDIO_DATA_SIZE);
            mkPacket(packet, MSG_MIC, (unsigned short)AUDIO_DATA_SIZE, 0, buf);
            if(audiooutBuffer.queue.size() == audiooutBuffer.bufferSize) {
                audiooutBuffer.queueMutex.lock();
                audiooutBuffer.bufferNotFull.wait(&audiooutBuffer.queueMutex);
                audiooutBuffer.queueMutex.unlock();
            }
            audiooutBuffer.bufferPacket(packet);
            availableData -= AUDIO_DATA_SIZE;
        } else {
            inbuf_->read(buf+HDR_SIZE, availableData);
            mkPacket(packet, MSG_MIC, (unsigned short)availableData, 0, buf);
            if(audiooutBuffer.queue.size() == audiooutBuffer.bufferSize) {
                audiooutBuffer.queueMutex.lock();
                audiooutBuffer.bufferNotFull.wait(&audiooutBuffer.queueMutex);
                audiooutBuffer.queueMutex.unlock();
            }
            audiooutBuffer.bufferPacket(packet);
            availableData = 0;
        }
    }
}

void audioin::mkVoiceHdr(char* buf) {
    int frequency = 44100;
    short channels = 2;
    short sampleSize = 16;

    memcpy(buf+24, &frequency, 4);
    memcpy(buf+22, &channels, 2);
    memcpy(buf+34, &sampleSize, 2);
}
void audioin::writeReady(qint64 bytes) {
    numToRead_ = bytes;
}
