/**
 * Copyright (c) 2011-2012 Nokia Corporation.
 * All rights reserved.
 *
 * Part of the Qt GameEnabler.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */

#include "audiomixer.h"
#include <memory.h>
#include "trace.h" // For debug macros

using namespace GE;


/*!
  \class AudioMixer
  \brief An AudioSource capable of combining all of its child sources into
         a single audio stream.
*/


/*!
  Constructor.
*/
AudioMixer::AudioMixer(QObject *parent)
    : AudioSource(parent),
      m_mixingBuffer(0),
      m_effect(0),
      m_mixingBufferLength(0),
      m_fixedGeneralVolume((int)GEMaxAudioVolumeValue)
{
    DEBUG_INFO(this);
}


/*!
  Destructor.
*/
AudioMixer::~AudioMixer()
{
    destroyList();

    if (m_mixingBuffer) {
        delete [] m_mixingBuffer;
        m_mixingBuffer = 0;
    }
}


/*!
  Returns the absolute volume.
*/
float AudioMixer::absoluteVolume() const
{
    return (float)m_fixedGeneralVolume / 4096.0f;
}


/*!
  Returns the general volume.
*/
float AudioMixer::generalVolume()
{
    return (float)m_fixedGeneralVolume *
           (float)audioSourceCount() / GEMaxAudioVolumeValue;
}


/*!
  Adds \a source to the list of audio sources. Returns true if the given audio
  source was added into the list, false otherwise.
*/
bool AudioMixer::addAudioSource(AudioSource *source)
{
    if (!source) {
        // Invalid argument!
        DEBUG_INFO("The given source is NULL!");
        return false;
    }

    QMutexLocker locker(&m_mutex);
    Q_UNUSED(locker); // To prevent warnings.
    m_sourceList.push_back(source);
    return true;
}


/*!
  Removes \a source from the list of audio sources. Returns true if
  found and removed, false otherwise.

  Note: The removed item is not deleted!
*/
bool AudioMixer::removeAudioSource(AudioSource *source)
{
    QMutexLocker locker(&m_mutex);
    Q_UNUSED(locker); // To prevent warnings
    return m_sourceList.removeOne(source);
}


/*!
  Destroys all the sources in the list.
*/
void AudioMixer::destroyList()
{
    QMutexLocker locker(&m_mutex);
    Q_UNUSED(locker); // To prevent warnings

    QList<AudioSource*>::iterator iter;

    for (iter = m_sourceList.begin(); iter != m_sourceList.end(); iter++) {
        delete *iter;
    }

    m_sourceList.clear();
}


/*!
  Returns the audio source list count.
*/
int AudioMixer::audioSourceCount()
{
    QMutexLocker locker(&m_mutex);
    Q_UNUSED(locker); // To prevent warnings
    return m_sourceList.count();
}


/*!
  From AudioSource.

  Mix the requested amount of samples from all mixer's audiosources
  into a single buffer.
*/
int AudioMixer::pullAudio(AUDIO_SAMPLE_TYPE *target, int bufferLength)
{
    QMutexLocker locker(&m_mutex);
    Q_UNUSED(locker); // To prevent warnings

    if ((m_sourceList.isEmpty() && m_effect.isNull())
             || bufferLength <= 0) {
        return 0;
    }

    if (m_mixingBufferLength < bufferLength) {
        if (m_mixingBuffer) {
            delete [] m_mixingBuffer;
        }

        m_mixingBufferLength = bufferLength;
        m_mixingBuffer = new AUDIO_SAMPLE_TYPE[m_mixingBufferLength];
    }

    memset(target, 0, sizeof(AUDIO_SAMPLE_TYPE) * bufferLength);

    AUDIO_SAMPLE_TYPE *t;
    AUDIO_SAMPLE_TYPE *t_target;
    AUDIO_SAMPLE_TYPE *s;

    QList<AudioSource*>::iterator iter(m_sourceList.begin());

    while (iter != m_sourceList.end()) {
        if (!(*iter)) {
            // NULL pointer!
            DEBUG_INFO("Warning: stumbled on a null pointer!");
            continue;
        }

        // Process the list item.
        int mixed = (*iter)->pullAudio(m_mixingBuffer, bufferLength);

        if (mixed > 0) {
            // Mix to main.
            t = target;
            t_target = t + mixed;
            s = m_mixingBuffer;

            while (t != t_target) {
                *t += (((*s) * m_fixedGeneralVolume) >> 12);
                t++;
                s++;
            }
        }

        if ((*iter)->canBeDestroyed()) {
            // Auto-destroy the current audio source.
            //
            // Note: The auto-destroy feature is undergoing testing and may
            // cause unpredictable crashes with some use cases!
            delete *iter;
            iter = m_sourceList.erase(iter);
        }
        else {
            iter++;
        }
    }

    if (!m_effect.isNull()) {
        return m_effect->process(target, bufferLength);
    }

    //DEBUG_INFO("Done, will return buffer length: " << bufferLength);
    return bufferLength;
}


/*!
  Sets \a volume as the absolute volume.
*/
void AudioMixer::setAbsoluteVolume(float volume)
{
    DEBUG_INFO(volume);
    m_fixedGeneralVolume = GEMaxAudioVolumeValue * volume;
    emit absoluteVolumeChanged(m_fixedGeneralVolume);
}


/*!
  Sets \a volume as the general volume, relative to the channel count
  (audio source count).
*/
void AudioMixer::setGeneralVolume(float volume)
{
    const int sourceCount(audioSourceCount());

    DEBUG_INFO(volume);

    // Safety checks for possible division by zero error.
    if (volume == 0) {
        m_fixedGeneralVolume = 0.0f;
    }
    else if (sourceCount) {
        m_fixedGeneralVolume =
            (GEMaxAudioVolumeValue / (float)audioSourceCount() * volume);
    }

    emit generalVolumeChanged(m_fixedGeneralVolume);
}

