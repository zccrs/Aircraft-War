/**
 * Copyright (c) 2011-2012 Nokia Corporation.
 * All rights reserved.
 *
 * Part of the Qt GameEnabler.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */

#ifndef GEAUDIOBUFFER_H
#define GEAUDIOBUFFER_H

#include "geglobal.h"
#include "audiosourceif.h"

// Forward declarations
class QFile;


namespace GE {

// Forward declarations (inside GE namespace)
class AudioBuffer;
class AudioBufferPlayInstance;
class AudioMixer;

// Prototype function for audio sampling
typedef AUDIO_SAMPLE_TYPE(*SAMPLE_FUNCTION_TYPE)(AudioBuffer *buffer,
                                                 int pos,
                                                 int channel);


class Q_GE_EXPORT AudioBuffer : public QObject
{
public:
    explicit AudioBuffer(QObject *parent = 0);
    virtual ~AudioBuffer();
    static AudioBuffer *load(QString fileName, QObject *parent = 0);
    static AudioBuffer *loadWav(QString fileName, QObject *parent = 0);
    static AudioBuffer *loadOgg(QString fileName, QObject *parent = 0);

public:
    void reallocate(int length);

    // Getters for raw sample data and sample details
    inline void* getRawData() { return m_data; }
    inline int getDataLength() { return m_dataLength; }
    inline int getBytesPerSample() { return (m_bitsPerSample >> 3); }
    inline int getBitsPerSample() { return m_bitsPerSample; }
    inline int getSamplesPerSec() { return m_samplesPerSec; }
    inline short getNofChannels() { return m_nofChannels; }
    inline SAMPLE_FUNCTION_TYPE getSampleFunction() { return m_sampleFunction; }

    // Static implementations of sample functions
    static AUDIO_SAMPLE_TYPE sampleFunction8bitMono(
        AudioBuffer *buffer, int pos, int channel);
    static AUDIO_SAMPLE_TYPE sampleFunction16bitMono(
        AudioBuffer *buffer, int pos, int channel);
    static AUDIO_SAMPLE_TYPE sampleFunction32bitMono(
        AudioBuffer *buffer, int pos, int channel);
    static AUDIO_SAMPLE_TYPE sampleFunction8bitStereo(
        AudioBuffer *buffer, int pos, int channel);
    static AUDIO_SAMPLE_TYPE sampleFunction16bitStereo(
        AudioBuffer *buffer, int pos, int channel);
    static AUDIO_SAMPLE_TYPE sampleFunction32bitStereo(
        AudioBuffer *buffer, int pos, int channel);

    AudioBufferPlayInstance *playWithMixer(GE::AudioMixer &mixer);

protected:
    static AudioBuffer *loadWav(QFile &wavFile, QObject *parent = 0);
    static bool setSampleFunction(AudioBuffer &buffer);

    bool readOgg(QString &fileName);

protected: // Data
    SAMPLE_FUNCTION_TYPE m_sampleFunction;
    void *m_data; // Owned
    int m_dataLength; // In bytes
    short m_nofChannels;
    short m_bitsPerSample;
    bool m_signedData;
    int m_samplesPerSec;
};

} // namespace GE

#endif // GEAUDIOBUFFER_H
