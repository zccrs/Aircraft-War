/**
 * Copyright (c) 2012 Nokia Corporation.
 * All rights reserved.
 *
 * Part of the Qt GameEnabler.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */

#ifndef GEOggSource_H
#define GEOggSource_H

#include <QPointer>
#include "geglobal.h"
#include "audiosourceif.h"
#include "audioeffect.h"

namespace GE {

// Forward declarations
class VorbisDecoder;

class Q_GE_EXPORT VorbisSource : public AudioSource
{
    Q_OBJECT

public:
    VorbisSource(QString filename, QObject *parent = 0);
    virtual ~VorbisSource();

public:
    inline bool isFinished() const { return m_finished; }
    inline void setDestroyWhenFinished(bool set) { m_destroyWhenFinished = set; }
    inline bool destroyWhenFinished() const { return m_destroyWhenFinished; }

public: // From AudioSource
    bool canBeDestroyed();
    int pullAudio(AUDIO_SAMPLE_TYPE *target, int bufferLength);

public slots:
    void play();
    void stop();
    void setSpeed(float speed);
    void seek(quint64 samplePos);
    quint64 position();
    quint64 length();
    void setLeftVolume(float volume);
    void setRightVolume(float volume);

protected:
    int mixBlock(AUDIO_SAMPLE_TYPE *target, int bufferLength);

signals:
    void finished();

protected: // Data
    VorbisDecoder *m_decoder;
    bool m_finished;
    bool m_destroyWhenFinished;
    qint64 m_fixedPos;
    int m_fixedInc;
    int m_fixedLeftVolume;
    int m_fixedRightVolume;
    int m_loopCount;
};

} // namespace GE

#endif // GEOggSource_H
