/**
 * Copyright (c) 2011-2012 Nokia Corporation.
 * All rights reserved.
 *
 * Part of the Qt GameEnabler.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */

#ifndef DECLARATIVEECHOEFFECT_H
#define DECLARATIVEECHOEFFECT_H

#include "declarativeaudioeffect.h"

namespace GE {

class DeclarativeEchoEffect : public DeclarativeAudioEffect
{
    Q_OBJECT
    Q_PROPERTY(float delay READ delay WRITE setDelay)
    Q_PROPERTY(float decay READ decay WRITE setDecay)

public:
    explicit DeclarativeEchoEffect(QDeclarativeItem *parent = 0);
    virtual ~DeclarativeEchoEffect();

public slots:
    float delay();
    void setDelay(float delay);

    float decay();
    void setDecay(float decay);
};

} // namespace GE

QML_DECLARE_TYPE(GE::DeclarativeEchoEffect)

#endif // DECLARATIVEECHOEFFECT_H

