#include "audioout.h"
#include "../network/audiotransfer.h"
#include "../network/network.h"
void audioout::setupParams() {
    format_.setFrequency(frequency_);
    format_.setChannels(channels_);
    format_.setSampleSize(sampleSize_);
    format_.setCodec("audio/pcm");
    format_.setByteOrder(QAudioFormat::LittleEndian);
    format_.setSampleType(QAudioFormat::SignedInt);
}

int audioout::getParams(const char* params) {
    if(memcmp(params, hdr_, HDR_SIZE) != 0) {
        memcpy(hdr_, params, HDR_SIZE);
        memcpy(&frequency_, hdr_+24, 4);
        memcpy(&byteRate_, hdr_+28, 4);
        memcpy(&channels_, hdr_+22, 2);
        memcpy(&sampleSize_, hdr_+34, 2);
        if((byteRate_) != 0) {
            pause_ = ((double)1/(((double)byteRate_)/((double)AUDIO_DATA_SIZE)));
        }
        return 1;
    }
    return 0;
}

void audioout::createAudioDev() {
    QAudioDeviceInfo i(QAudioDeviceInfo::defaultOutputDevice());
    if(!i.isFormatSupported(format_)) {
        format_ = i.nearestFormat(format_);
    }
    if(output_ == NULL) {
        output_ = new QAudioOutput(format_);
        output_->setBufferSize(6000000);
        buffer_ = output_->start();
    }
}

void audioout::destroyAudioDev() {
    delete output_;
}

void audioout::playSound(char* sound){
    buffer_->write(sound+48, AUDIO_DATA_SIZE);
    if((pause_ * 1000) > 0) {
        Sleep(pause_*1000);
    }
}
