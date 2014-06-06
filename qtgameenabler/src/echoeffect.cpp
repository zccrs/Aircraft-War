/**
 * Copyright (c) 2011-2012 Nokia Corporation.
 * All rights reserved.
 *
 * Part of the Qt GameEnabler.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */

#include "echoeffect.h"

#include <QMutexLocker>
#include <math.h>

#include "trace.h"

using namespace GE;

EchoEffect::EchoEffect(QObject *parent)
    : AudioEffect(parent),
      m_delayBuffer(NULL)
{
    DEBUG_INFO(this);

    setDelay(1.0f);
    setDecay(1.0f);
    setupBuffer();
}

EchoEffect::~EchoEffect()
{
    delete []m_delayBuffer;
}

void EchoEffect::setDelay(float value)
{
    m_delay = value;
    setupBuffer();
}

float EchoEffect::delay()
{
    return m_delay;
}

void EchoEffect::setDecay(float value)
{
    m_decay = value;
    m_fixedDecay = value * 4096;
}

float EchoEffect::decay()
{
    return m_decay;
}

void EchoEffect::flush()
{
    setupBuffer();
    AudioEffect::flush();
}

void EchoEffect::setupBuffer()
{
    DEBUG_POINT;

    QMutexLocker locker(&m_mutex);
    Q_UNUSED(locker);

    m_delayInSamples = AUDIO_CHANNELS * (int)(AUDIO_FREQUENCY * m_delay);

    DEBUG_INFO("Setting delay to" << m_delayInSamples << "samples");

    delete []m_delayBuffer;
    m_delayBuffer = new AUDIO_SAMPLE_TYPE[m_delayInSamples];
    memset(m_delayBuffer, 0, m_delayInSamples * sizeof(AUDIO_SAMPLE_TYPE));
    m_index = 0;
}

int EchoEffect::process(AUDIO_SAMPLE_TYPE *target, int bufferLength)
{
    QMutexLocker locker(&m_mutex);
    Q_UNUSED(locker);

    if (!m_delayBuffer) {
        setupBuffer();

        if (!m_delayBuffer) {
            return 0;
        }
    }

    AUDIO_SAMPLE_TYPE *max_p = target + bufferLength;
    AUDIO_SAMPLE_TYPE *p = target;

    while (p < max_p) {
        *p += (AUDIO_SAMPLE_TYPE)((int)m_delayBuffer[m_index]
                                  * m_fixedDecay >> 12);
        m_delayBuffer[m_index++] = *p;

        if (m_index >= m_delayInSamples) {
            m_index = 0;
        }

        p++;
    }

    return AudioEffect::process(target, bufferLength);
}
