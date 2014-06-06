/**
 * Copyright (c) 2011-2012 Nokia Corporation.
 * All rights reserved.
 *
 * Part of the Qt GameEnabler.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */

#include "declarativeaudioeffect.h"
#include "trace.h"

using namespace GE;

DeclarativeAudioEffect::DeclarativeAudioEffect(QDeclarativeItem *parent)
    : QDeclarativeItem(parent),
      m_effect(NULL)
{
    DEBUG_INFO(this);
}

DeclarativeAudioEffect::~DeclarativeAudioEffect()
{
    DEBUG_POINT;
}

void DeclarativeAudioEffect::flush()
{
    m_effect->flush();
}

