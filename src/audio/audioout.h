#ifndef AUDIOOUT_H
#define AUDIOOUT_H
#include <QObject>
#include <QAudioOutput>
#include <QAudioFormat>
#include "../network/audiotransfer.h"
class audioout: public QObject {
    Q_OBJECT

    private:
        QAudioOutput* output_;
        QAudioFormat format_;
        QIODevice* buffer_;

        int frequency_;
        short channels_;
        short sampleSize_;
        char hdr_[HDR_SIZE];
    public:
        explicit audioout(){}
        virtual ~audioout(){}
        int getParams(char* params);
        void setupParams();
        void createAudioDev();
        void destroyAudioDev();
    public slots:
        void playSound(char* sound);
};
#endif
