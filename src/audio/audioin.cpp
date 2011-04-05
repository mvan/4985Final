#include "audioin.h"
#include "../network/externs.h"
#include "../network/network.h"
void audioin::setupParams() {
    format_.setFrequency(6800);
    format_.setChannels(1);
    format_.setSampleSize(8);
    format_.setCodec("audio/pcm");
    format_.setByteOrder(QAudioFormat::LittleEndian);
    format_.setSampleType(QAudioFormat::SignedInt);
}

void audioin::createAudioDev() {
    QAudioDeviceInfo i(QAudioDeviceInfo::defaultOutputDevice());
    if(!i.isFormatSupported(format_)) {
        return;
    } else {
        input_ = new QAudioOutput(format_);
        buffer_ = input_->start();
        inbuf_ = (QBuffer*)buffer_;
    }
}

void audioin::destroyAudioDev() {
    delete input_;
}

void audioin::readSound(){
    char buf[DATA_SIZE];
    char packet[PACKETSIZE];
    inbuf_->read(buf, DATA_SIZE);
    mkPacket(packet, MSG_MIC, DATA_SIZE, 0, buf);
    audiooutBuffer.bufferPacket(packet);
}
