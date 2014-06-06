/**
 * Copyright (c) 2011-2012 Nokia Corporation.
 * All rights reserved.
 *
 * Part of the Qt GameEnabler.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */

#ifndef GEAUDIOMIXER_H
#define GEAUDIOMIXER_H

#include <QMutex>
#include "geglobal.h"
#include "audiosourceif.h"
#include "audioeffect.h"

namespace GE {

class Q_GE_EXPORT AudioMixer : public AudioSource
{
    Q_OBJECT

public:
    explicit AudioMixer(QObject *parent = 0);
    virtual ~AudioMixer();

public:
    float absoluteVolume() const;
    float generalVolume();
    bool addAudioSource(AudioSource *source);
    bool removeAudioSource(AudioSource *source);
    void destroyList();
    int audioSourceCount();
    void setEffect(AudioEffect *effect) { m_effect = effect; }

public: // From AudioSource
    int pullAudio(AUDIO_SAMPLE_TYPE *target, int bufferLength);

public slots:
    void setAbsoluteVolume(float volume);
    void setGeneralVolume(float volume);

signals:
    void absoluteVolumeChanged(float volume);
    void generalVolumeChanged(float volume);

protected: // Data
    QList<AudioSource*> m_sourceList; // Owned
    AUDIO_SAMPLE_TYPE *m_mixingBuffer; // Owned
    QPointer<AudioEffect> m_effect; // Not owned
    QMutex m_mutex;
    int m_mixingBufferLength;
    int m_fixedGeneralVolume;
};

} // namespace GE

#endif // GEAUDIOMIXER_H
