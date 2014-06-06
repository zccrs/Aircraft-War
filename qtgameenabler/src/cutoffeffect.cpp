/**
 * Copyright (c) 2011-2012 Nokia Corporation.
 * All rights reserved.
 *
 * Part of the Qt GameEnabler.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */

#include "cutoffeffect.h"

#include <QMutexLocker>
#include <math.h>

#include "trace.h"

using namespace GE;


CutOffEffect::CutOffEffect(QObject *parent)
    : AudioEffect(parent),
      m_lp(NULL),
      m_hp(NULL),
      m_bp(NULL)
{
    DEBUG_INFO(this);
    setResonance(1.0f);
    setCutOff(1.0f);
    setup();
}

CutOffEffect::~CutOffEffect()
{
    DEBUG_POINT;
    delete []m_lp;
    delete []m_hp;
    delete []m_bp;
}

void CutOffEffect::setCutOff(float value)
{
    m_cutOff = value;
    m_fixedCutOff = value * 4096.0f;
}

float CutOffEffect::cutOff()
{
    return m_cutOff;
}

void CutOffEffect::setResonance(float value)
{
    m_resonance = value;
    m_fixedResonance = value * 4096.0f;
}

float CutOffEffect::resonance()
{
    return m_resonance;
}

void CutOffEffect::flush()
{
    setup();
    AudioEffect::flush();
}

void CutOffEffect::setup()
{
    QMutexLocker locker(&m_mutex);
    Q_UNUSED(locker);

    delete []m_lp;
    m_lp = new int[AUDIO_CHANNELS];
    memset(m_lp, 0, sizeof(int) * AUDIO_CHANNELS);
    delete []m_bp;
    m_bp = new int[AUDIO_CHANNELS];
    memset(m_bp, 0, sizeof(int) * AUDIO_CHANNELS);
    delete []m_hp;
    m_hp = new int[AUDIO_CHANNELS];
    memset(m_hp, 0, sizeof(int) * AUDIO_CHANNELS);
}

int CutOffEffect::process(AUDIO_SAMPLE_TYPE *target, int bufferLength)
{
    QMutexLocker locker(&m_mutex);
    Q_UNUSED(locker);

    AUDIO_SAMPLE_TYPE *p_max = target + bufferLength;
    AUDIO_SAMPLE_TYPE *p = target;
    int i, input;
    qint64 pos = 0;

    while (p < p_max) {
        for (i = 0; i < AUDIO_CHANNELS; i++) {
            input = ((*p * (2047^(pos & 2047)) +
                    *(p + AUDIO_CHANNELS) *
                    (pos & 2047)) >> 11);
            m_lp[i] += ((m_bp[i] * m_fixedCutOff) >> 12);
            m_hp[i] = input - m_lp[i] - ((m_bp[i] * m_fixedResonance) >> 12);
            m_bp[i] += ((m_hp[i] * m_fixedCutOff) >> 12);

            input = m_lp[i];

            if (input < -32767) {
                input = -32767;
            }

            if (input > 32767) {
                input = 32767;
            }

            *(p++) = input;
        }

        pos += 2048;
    }

    return AudioEffect::process(target, bufferLength);
}

