/**
 * Copyright (c) 2011-2012 Nokia Corporation.
 * All rights reserved.
 *
 * Part of the Qt GameEnabler.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */

#ifndef ECHOEFFECT_H
#define ECHOEFFECT_H

#include <QObject>
#include <QMutex>
#include "geglobal.h"
#include "audioeffect.h"

namespace GE {

class Q_GE_EXPORT EchoEffect : public AudioEffect
{
    Q_OBJECT

public:
    explicit EchoEffect(QObject *parent = 0);
    virtual ~EchoEffect();

    void setDelay(float value);
    float delay();

    void setDecay(float value);
    float decay();

public:
    void flush();
    int process(AUDIO_SAMPLE_TYPE *target, int bufferLength);

private:
    void setupBuffer();

private:
    QMutex m_mutex;
    AUDIO_SAMPLE_TYPE *m_delayBuffer;
    float m_delay;
    float m_decay;
    int m_fixedDecay;
    int m_delayInSamples;
    int m_index;
};

} // namespace GE

#endif // ECHOEFFECT_H
