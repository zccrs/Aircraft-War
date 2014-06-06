/**
 * Copyright (c) 2011-2012 Nokia Corporation.
 * All rights reserved.
 *
 * Part of the Qt GameEnabler.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */

#include "audioeffect.h"
#include "trace.h"

using namespace GE;

AudioEffect::AudioEffect(QObject *parent)
    : QObject(parent),
      m_next(NULL)
{
    DEBUG_INFO(this);
}

AudioEffect::~AudioEffect()
{
    DEBUG_POINT;
}

AudioEffect *AudioEffect::next()
{
    return m_next;
}

void AudioEffect::linkTo(AudioEffect *next)
{
    DEBUG_POINT;

    if (!m_next.isNull()) {
        m_next->linkTo(next);
        return;
    }

   m_next = next;
}

void AudioEffect::flush()
{
    if (!m_next.isNull())
        m_next->flush();
}

int AudioEffect::process(AUDIO_SAMPLE_TYPE *target, int bufferLength)
{
    if (!m_next.isNull())
        bufferLength = m_next->process(target, bufferLength);

    return bufferLength;
}
