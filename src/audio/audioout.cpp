#include "audioout.h"
#include "../network/audiotransfer.h"
void audioout::setupParams() {
    format_.setFrequency(frequency_);
    format_.setChannels(channels_);
    format_.setSampleSize(sampleSize_);
    format_.setCodec("audio/pcm");
    format_.setByteOrder(QAudioFormat::LittleEndian);
    format_.setSampleType(QAudioFormat::SignedInt);
}

int audioout::getParams(char* params) {
    if(memcmp(params, hdr_, HDR_SIZE) != 0) {
        memcpy(hdr_, params, HDR_SIZE);
        memcpy(&frequency_, params+24, 4);
        memcpy(&channels_, params+22, 2);
        memcpy(&sampleSize_, params+34, 2);
        return 1;
    }
    return 0;
}

void audioout::createAudioDev() {
    if(output_ == NULL) {
        output_ = new QAudioOutput(format_);
        int i = output_->periodSize();
        output_->setBufferSize(6000000);
        buffer_ = output_->start();
    }
}

void audioout::destroyAudioDev() {
    delete output_;
}

void audioout::playSound(char* sound){
    buffer_->write(sound+48, AUDIO_DATA_SIZE);
}
