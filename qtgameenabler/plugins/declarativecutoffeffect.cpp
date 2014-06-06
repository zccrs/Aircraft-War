/**
 * Copyright (c) 2011-2012 Nokia Corporation.
 * All rights reserved.
 *
 * Part of the Qt GameEnabler.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */

#include "declarativecutoffeffect.h"
#include "cutoffeffect.h"
#include "trace.h"

using namespace GE;

DeclarativeCutOffEffect::DeclarativeCutOffEffect(QDeclarativeItem *parent)
    : DeclarativeAudioEffect(parent)
{
    DEBUG_INFO(this);
    m_effect = new CutOffEffect(parent);
}

DeclarativeCutOffEffect::~DeclarativeCutOffEffect()
{
    DEBUG_POINT;
    delete m_effect;
}

void DeclarativeCutOffEffect::setCutOff(float value)
{
    DEBUG_INFO(value);
    qobject_cast<CutOffEffect*>(m_effect)->setCutOff(value);
    emit cutOffChanged();
}

float DeclarativeCutOffEffect::cutOff()
{
    DEBUG_POINT;
    return qobject_cast<CutOffEffect*>(m_effect)->cutOff();
}

void DeclarativeCutOffEffect::setResonance(float value)
{
    DEBUG_INFO(value);
    qobject_cast<CutOffEffect*>(m_effect)->setResonance(value);
    emit resonanceChanged();
}

float DeclarativeCutOffEffect::resonance()
{
    DEBUG_POINT;
    return qobject_cast<CutOffEffect*>(m_effect)->resonance();
}

