/**
 * Copyright (c) 2011-2012 Nokia Corporation.
 * All rights reserved.
 *
 * Part of the Qt GameEnabler.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */

#ifndef GEPUSHAUDIOOUT_H
#define GEPUSHAUDIOOUT_H

#include <QAudioOutput>
#include <QThread>
#include <QIODevice>
#include <QPointer>
#include "geglobal.h"
#include "audioout.h"

namespace GE {

class Q_GE_EXPORT PushAudioOut : public QThread,
                                 public AudioOut
{
    Q_OBJECT

public:
    PushAudioOut(AudioSource *source, QObject *parent = 0);
    virtual ~PushAudioOut();

public:
    bool needsManualTick() const { return m_needsTick; }
    void tick();

protected: // From QThread
     virtual void run(); // For the threaded mode only!

private: // Data types
    enum ThreadStates {
        NotRunning = 0,
        DoRun = 1,
        DoExit = 2
    };

protected: // Data
    QAudioOutput *m_audioOutput; // Owned
    QIODevice *m_outTarget; // Not owned
    AUDIO_SAMPLE_TYPE *m_sendBuffer; // Owned
    int m_sendBufferSize;
    qint64 m_samplesMixed;
    int m_threadState;
    bool m_needsTick;
    QPointer<AudioSource> m_source; // Not owned
};

} // namespace GE

#endif // GEPUSHAUDIOOUT_H
