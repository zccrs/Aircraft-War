/**
 * Copyright (c) 2011-2012 Nokia Corporation.
 * All rights reserved.
 *
 * Part of the Qt GameEnabler.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */

#include "declarativeechoeffect.h"
#include "echoeffect.h"
#include "trace.h"

using namespace GE;

DeclarativeEchoEffect::DeclarativeEchoEffect(QDeclarativeItem *parent)
    : DeclarativeAudioEffect(parent)
{
    DEBUG_INFO(this);
    m_effect = new EchoEffect(parent);
}

DeclarativeEchoEffect::~DeclarativeEchoEffect()
{
    DEBUG_POINT;
    delete m_effect;
}

void DeclarativeEchoEffect::setDelay(float delay)
{
    DEBUG_POINT;
    qobject_cast<EchoEffect*>(m_effect)->setDelay(delay);
}

float DeclarativeEchoEffect::delay()
{
    DEBUG_POINT;
    return qobject_cast<EchoEffect*>(m_effect)->delay();
}

void DeclarativeEchoEffect::setDecay(float decay)
{
    DEBUG_POINT;
    qobject_cast<EchoEffect*>(m_effect)->setDecay(decay);
}

float DeclarativeEchoEffect::decay()
{
    DEBUG_POINT;
    return qobject_cast<EchoEffect*>(m_effect)->decay();
}

