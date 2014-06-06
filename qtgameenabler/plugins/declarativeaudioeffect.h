/**
 * Copyright (c) 2011-2012 Nokia Corporation.
 * All rights reserved.
 *
 * Part of the Qt GameEnabler.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */

#ifndef DECLARATIVEAUDIOEFFECT_H
#define DECLARATIVEAUDIOEFFECT_H

#include <QDeclarativeItem>
#include "audioeffect.h"
#include "declarativeaudiobuffer.h"

namespace GE {

class DeclarativeAudioEffect : public QDeclarativeItem
{
    Q_OBJECT

public:
    explicit DeclarativeAudioEffect(QDeclarativeItem *parent = 0);
    virtual ~DeclarativeAudioEffect();

public slots:
    void flush();

protected:
    AudioEffect *m_effect;

    friend class DeclarativeAudioMixer;
    friend class DeclarativeAudioBuffer;
};

} // namespace GE

QML_DECLARE_TYPE(GE::DeclarativeAudioEffect)

#endif // DECLARATIVEAUDIOEFFECT_H

