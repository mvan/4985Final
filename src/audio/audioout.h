/**
  * Class: audioout
  *
  * Description:
  * This class has all the tools for setting up and sending sound
  * to the audio output device.
  */
#ifndef AUDIOOUT_H
#define AUDIOOUT_H
#include <QObject>
#include <QAudioOutput>
#include <QAudioFormat>
#include "../network/audiotransfer.h"
class audioout: public QObject {
    Q_OBJECT

private:
    /** AudioOutput object that performs operations for output. */
    QAudioOutput* output_;

    /** The format for the current audio output. */
    QAudioFormat format_;

    /** The buffer that output_ reads from to play sound. */
    QIODevice* buffer_;

    /** The frequency of the audio. (For format) */
    int frequency_;

    /** The byteRate of the audio. (For format) */
    int byteRate_;

    /** The number of milliseconds for the current audio data. */
    double pause_;

    /** The number of channels of the audio. (For format) */
    short channels_;

    /** The sample size of the audio. (For format) */
    short sampleSize_;

    /** The header for the audio. */
    char hdr_[HDR_SIZE];
public:
    explicit audioout():output_(NULL), pause_(0){}
    virtual ~audioout(){}

    /**
      * Function: getParams(const char* params)
      *
      * @return 1 if unsuccessful. 0 if successful.
      *
      * @param params the data to be parsed for format.
      *
      * Description:
      * Sets up the audio format parameters based on params.
      */
    int getParams(const char* params);

    /**
      * Function: setupParams()
      *
      * @return void
      *
      * Description:
      * Sets up the audio format parameters based on the standard wav type.
      */
    void setupParams();

    /**
      * Function: createAudioDev()
      *
      * @return void
      *
      * Description:
      * Sets up the default audio output device with the QAudioOutput
      * using the ouput buffer.
      */
    void createAudioDev();

    /**
      * Function: destroyAudioDev()
      *
      * @return void
      *
      * Description:
      * Cleans up the AudioOutput and output buffer.
      */
    void destroyAudioDev();

public slots:
    /**
      * Function: playSound(char* sound)
      *
      * @return void
      *
      * @param sound
      *
      * Description:
      * Sends the sound data to the output device.
      */
    void playSound(char* sound);
};
#endif
