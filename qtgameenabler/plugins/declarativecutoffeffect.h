/**
 * Copyright (c) 2011-2012 Nokia Corporation.
 * All rights reserved.
 *
 * Part of the Qt GameEnabler.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */

#ifndef DECLARATIVECUTOFFEFFECT_H
#define DECLARATIVECUTOFFEFFECT_H

#include "declarativeaudioeffect.h"

namespace GE {

class DeclarativeCutOffEffect : public DeclarativeAudioEffect
{
    Q_OBJECT
    Q_PROPERTY(float cutOff READ cutOff WRITE setCutOff NOTIFY cutOffChanged)
    Q_PROPERTY(float resonance READ resonance WRITE setResonance
        NOTIFY resonanceChanged)

public:
    explicit DeclarativeCutOffEffect(QDeclarativeItem *parent = 0);
    virtual ~DeclarativeCutOffEffect();

public slots:
    void setCutOff(float value);
    float cutOff();

    void setResonance(float value);
    float resonance();

signals:
    void cutOffChanged();
    void resonanceChanged();
};

} // namespace GE

QML_DECLARE_TYPE(GE::DeclarativeCutOffEffect)

#endif // DECLARATIVECUTOFFEFFECT_H

