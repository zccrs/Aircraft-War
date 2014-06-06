/**
 * Copyright (c) 2011-2012 Nokia Corporation.
 * All rights reserved.
 *
 * Part of the Qt GameEnabler.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */

#ifndef GEAUDIOBUFFERPLAYINSTANCE_H
#define GEAUDIOBUFFERPLAYINSTANCE_H

#include <QPointer>
#include "geglobal.h"
#include "audiosourceif.h"
#include "audioeffect.h"

namespace GE {

// Forward declarations
class AudioBuffer;


class Q_GE_EXPORT AudioBufferPlayInstance : public AudioSource
{
    Q_OBJECT

public:
    AudioBufferPlayInstance(AudioBuffer *buffer = 0, QObject *parent = 0);
    virtual ~AudioBufferPlayInstance();

public:
    bool isPlaying() const;
    inline bool isFinished() const { return m_finished; }
    inline void setDestroyWhenFinished(bool set) { m_destroyWhenFinished = set; }
    inline bool destroyWhenFinished() const { return m_destroyWhenFinished; }

public: // From AudioSource
    bool canBeDestroyed();
    int pullAudio(AUDIO_SAMPLE_TYPE *target, int bufferLength);

public slots:
    void playBuffer(AudioBuffer *buffer, int loopCount = 0);
    void playBuffer(AudioBuffer *buffer,
                    float volume,
                    float speed,
                    int loopCount = 0);
    void stop();
    void setLoopCount(int count);
    void setSpeed(float speed);
    void seek(unsigned int samplePos);
    unsigned int position();
    void setLeftVolume(float volume);
    void setRightVolume(float volume);
    void setEffect(AudioEffect *effect) { m_effect = effect; }

protected:
    int mixBlock(AUDIO_SAMPLE_TYPE *target, int bufferLength);

signals:
    void finished();

protected: // Data
    QPointer<AudioBuffer> m_buffer; // Not owned
    QPointer<AudioEffect> m_effect; // Not owned
    bool m_finished;
    bool m_destroyWhenFinished;
#ifdef QTGAMEENABLER_SUPPORT_LONG_SAMPLES
    quint64 m_fixedPos;
#else
    unsigned int m_fixedPos;
#endif
    int m_fixedInc;
    int m_fixedLeftVolume;
    int m_fixedRightVolume;
    int m_loopCount;
};

} // namespace GE

#endif // GEAUDIOBUFFERPLAYINSTANCE_H
