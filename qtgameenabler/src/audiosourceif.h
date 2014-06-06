/**
 * Copyright (c) 2011-2012 Nokia Corporation.
 * All rights reserved.
 *
 * Part of the Qt GameEnabler.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */

#ifndef GEINTERFACES_H
#define GEINTERFACES_H

#include <QObject>


namespace GE {

#define AUDIO_FREQUENCY 11025
#define AUDIO_SAMPLE_TYPE short
#define AUDIO_SAMPLE_BITS 16
#define AUDIO_CHANNELS 2

// Constants
const float GEMaxAudioVolumeValue(4096.0f);


class AudioSource : public QObject
{
    Q_OBJECT

public:
    explicit AudioSource(QObject *parent = 0);
    virtual ~AudioSource();

public:
    virtual bool canBeDestroyed();
    virtual int pullAudio(AUDIO_SAMPLE_TYPE *target, int bufferLength ) = 0;
};

} // namespace GE

#endif // GEINTERFACES_H
