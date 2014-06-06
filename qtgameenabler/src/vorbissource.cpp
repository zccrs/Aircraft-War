/**
 * Copyright (c) 2012 Nokia Corporation.
 * All rights reserved.
 *
 * Part of the Qt GameEnabler.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */

#include "vorbissource.h"
#include "audiobuffer.h"
#include "audioeffect.h"
#include "trace.h"
#include "vorbisdecoder.h"

#include <QDebug>

using namespace GE;

// Constants
const float GEMaxAudioSpeedValue(4096.0f);
const float GEDefaultAudioVolume(1.0f); // 1.0 => 100 %
const float GEDefaultAudioSpeed(1.0f); // 1.0 => 100 %


/*!
 * \class VorbisSource
 * \brief An AudioSource instance capable of playing Ogg Vorbis files.
 */


/*!
  Constructor.
*/
VorbisSource::VorbisSource(QString filename, QObject *parent)
    : AudioSource(parent),
      m_finished(true),
      m_destroyWhenFinished(false),
      m_fixedPos(0),
      m_fixedInc(0),
      m_fixedLeftVolume((int)GEMaxAudioVolumeValue),
      m_fixedRightVolume((int)GEMaxAudioVolumeValue),
      m_loopCount(0)
{
    DEBUG_POINT;
    m_decoder = new VorbisDecoder(true, this);
    m_decoder->load(filename);
}


/*!
  Destructor.
*/
VorbisSource::~VorbisSource()
{
    DEBUG_POINT;
    delete m_decoder;
}

/*!
  From AudioSource.

  The framework will use this to know whether this AudioSource can be
  destroyed or not.
*/
bool VorbisSource::canBeDestroyed()
{
    return false;
}

/*!
  From AudioSource.

  Returns an audio stream from the current sample.
*/
int VorbisSource::pullAudio(AUDIO_SAMPLE_TYPE *target, int bufferLength)
{
    if (m_finished)
        return 0;

    unsigned int channelLength(m_decoder->decodedLength() - 2);
    unsigned int samplesToWrite(bufferLength / 2);
    unsigned int amount(0);
    unsigned int totalMixed(0);

    while (samplesToWrite > 0) {
        unsigned int samplesLeft;

        if (m_fixedInc == 0) {
            // No speed set. Will lead to division by zero error if not set.
            setSpeed(GEDefaultAudioSpeed);
        }

        samplesLeft = m_fixedInc > 0 ? channelLength - (m_fixedPos >> 12) :
            (m_fixedPos >> 12);

        // This is how much we can mix at least.
        unsigned int maxMixAmount = (int)(((long long int)(samplesLeft) << 12) /
                                 qAbs(m_fixedInc));

        if (maxMixAmount > samplesToWrite) {
            maxMixAmount = samplesToWrite;
        }

        if (maxMixAmount > 0) {
            amount = mixBlock(target+totalMixed * 2, maxMixAmount);

            if (amount == 0) {
                // Error!
                break;
            }

            totalMixed += amount;
        }
        else {
            amount = 0;
            m_fixedPos = m_fixedInc > 0 ? channelLength<<12 : 0;
        }

        // The sample ended. Check the looping variables and see what to do.
        if ((m_fixedPos >> 12) >= channelLength || m_fixedPos <= 0) {
            m_fixedPos += m_fixedInc > 0 ? -((qint64)channelLength << 12) :
                ((qint64)channelLength << 12);

            if (m_loopCount > 0)
                m_loopCount--;

            if (m_loopCount == 0) {
                // No more loops, stop the sample and return the amount of
                // samples already mixed.
                stop();
                break;
            }
        }

        samplesToWrite -= amount;

        if (samplesToWrite < 1)
            break;
    }

    return totalMixed * 2;
}

/*!
  Play/resume playback.
*/
void VorbisSource::play()
{
    m_finished = false;
}

/*!
  Stop/pause playback. Calling play() after stop() continues playback from the
  position where the playback was stopped.
*/
void VorbisSource::stop()
{
    m_finished = true;
}

/*!
  Sets \a speed as the speed of which the buffer is played in. The given
  argument value should not be zero. 1.0 indicates 100 % forward speed, -1.0
  100 % reverse speed.
*/
void VorbisSource::setSpeed(float speed)
{
    m_fixedInc = (int)(((float)m_decoder->fileInfo()->sample_rate *
        GEMaxAudioSpeedValue * speed) / (float)AUDIO_FREQUENCY);
    
    if (!m_fixedInc)
        m_fixedInc = 1;
}

/*!
  Sets playback position to given value. The given argument is in units of PCM
  audio samples per channel. 0 indicates beginning of the buffer and -1 the
  end of the buffer.
*/
void VorbisSource::seek(quint64 samplePos)
{
    const unsigned int channelLength = m_decoder->decodedLength();

    if (samplePos > channelLength)
        samplePos = channelLength;

    m_fixedPos = (quint64)samplePos << 12;
}

/*!
  Return the current playback position in units of PCM audio samples per
  channel.
*/
quint64 VorbisSource::position()
{
    return (m_fixedPos >> 12);
}

/*!
  Return the length of the stream in units of PCM audio samples per
  channel.
*/
quint64 VorbisSource::length()
{
    return m_decoder->decodedLength();
}

/*!
  Sets \a volume for the left channel. The given argument value should be
  between 0.0 and 1.0 since 1.0 indicates 100 %.
*/
void VorbisSource::setLeftVolume(float volume)
{
    m_fixedLeftVolume = (int)(GEMaxAudioVolumeValue * volume);
}


/*!
  Sets \a volume for the right channel. The given argument value should be
  between 0.0 and 1.0 since 1.0 indicates 100 %.
*/
void VorbisSource::setRightVolume(float volume)
{
    m_fixedRightVolume = (int)(GEMaxAudioVolumeValue * volume);
}

int VorbisSource::mixBlock(AUDIO_SAMPLE_TYPE *target, int samplesToMix)
{
    AUDIO_SAMPLE_TYPE *t_target = target + samplesToMix * 2;
    qint64 sourcepos(0);

    if (m_decoder->fileInfo()->channels == 2) {
        // Stereo
        while (target != t_target) {
            sourcepos = m_fixedPos >> 12;
            *target++ = ((((m_decoder->at(sourcepos * 2) *
                            (4096 - (m_fixedPos & 4095)) +
                            m_decoder->at((sourcepos + 1) * 2) *
                            (m_fixedPos & 4095)) >> 12) *
                          m_fixedLeftVolume) >> 12);

            *target++ = ((((m_decoder->at(sourcepos * 2 + 1) *
                            (4096 - (m_fixedPos & 4095)) +
                            m_decoder->at((sourcepos + 1) * 2 + 1) *
                            (m_fixedPos & 4095) ) >> 12) *
                          m_fixedRightVolume) >> 12);
            m_fixedPos += m_fixedInc;
        }
    }
    else {
        // Mono
        int temp(0);

        while (target != t_target) {
            sourcepos = m_fixedPos >> 12;
            temp = ((m_decoder->at(sourcepos) *
                     (4096 - (m_fixedPos & 4095)) +
                     m_decoder->at(sourcepos + 1) *
                     (m_fixedPos & 4095)) >> 12);
            *target++ = temp;
            *target++ = temp;
            m_fixedPos += m_fixedInc;
        }
    }

    return samplesToMix;
}
