/**
 * Copyright (c) 2011-2012 Nokia Corporation.
 * All rights reserved.
 *
 * Part of the Qt GameEnabler.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */

#ifndef GEAUDIOOUT_H
#define GEAUDIOOUT_H

#include <QAudioFormat>
#include "audiosourceif.h"

namespace GE {

const QString GEDefaultAudioCodec("audio/pcm");
const QAudioFormat::Endian GEByteOrder(QAudioFormat::LittleEndian);
const QAudioFormat::SampleType GESampleType(QAudioFormat::SignedInt);

class AudioOut
{
public:
    AudioOut() {}
    virtual ~AudioOut() {}

public:
    virtual bool needsManualTick() const { return false; };
    virtual void tick() {};
};

} // namespace GE

#endif // GEAUDIOOUT_H
