/**
 * Copyright (c) 2011-2012 Nokia Corporation.
 * All rights reserved.
 *
 * Part of the Qt GameEnabler.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */

#ifndef CUTOFFEFFECT_H
#define CUTOFFEFFECT_H

#include <QObject>
#include <QMutex>
#include "geglobal.h"
#include "audioeffect.h"

namespace GE {

class Q_GE_EXPORT CutOffEffect : public AudioEffect
{
    Q_OBJECT

public:
    explicit CutOffEffect(QObject *parent = 0);
    virtual ~CutOffEffect();

    void setCutOff(float value);
    float cutOff();

    void setResonance(float value);
    float resonance();

public:
    void flush();
    int process(AUDIO_SAMPLE_TYPE *target, int bufferLength);

private:
    void setup();

private:
    QMutex m_mutex;
    float m_cutOff;
    int m_fixedCutOff;
    float m_resonance;
    int m_fixedResonance;
    int *m_lp;
    int *m_hp;
    int *m_bp;
};

} // namespace GE

#endif // CUTOFFEFFECT_H

