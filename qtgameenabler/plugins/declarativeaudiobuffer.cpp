/**
 * Copyright (c) 2011-2012 Nokia Corporation.
 * All rights reserved.
 *
 * Part of the Qt GameEnabler.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */

#include "declarativeaudiobuffer.h"
#include "declarativeaudioeffect.h"
#include "trace.h"

using namespace GE;

DeclarativeAudioBuffer::DeclarativeAudioBuffer(QDeclarativeItem *parent)
    : QDeclarativeItem(parent),
      m_mixer(NULL),
      m_buffer(NULL),
      m_playInstance(NULL),
      m_loopCount(0),
      m_speed(1.0f),
      m_lVolume(1.0f),
      m_rVolume(1.0f)
{
    DEBUG_INFO(this);
    m_effects.clear();
}

DeclarativeAudioBuffer::~DeclarativeAudioBuffer()
{
    DEBUG_POINT;

    if (m_buffer)
        delete m_buffer;
}

void DeclarativeAudioBuffer::setSource(QString source)
{
    DEBUG_POINT;

    if (m_buffer) {
        stop();
        delete m_buffer;
        m_buffer = NULL;
    }

    m_source = source;
    m_buffer = AudioBuffer::load(m_source);
    if (!m_buffer)
        DEBUG_INFO("Failed to load" << m_source);
    else
        DEBUG_INFO(m_source << "loaded");
}

QString DeclarativeAudioBuffer::source() const
{
    DEBUG_POINT;
    return m_source;
}

void DeclarativeAudioBuffer::setMixer(DeclarativeAudioMixer *mixer)
{
    DEBUG_POINT;
    m_mixer = mixer;
}

DeclarativeAudioMixer *DeclarativeAudioBuffer::mixer() const
{
    DEBUG_POINT;
    return m_mixer;
}

QDeclarativeListProperty<DeclarativeAudioEffect>
    DeclarativeAudioBuffer::effectProperty()
{
    return QDeclarativeListProperty<DeclarativeAudioEffect>(this, &m_effects,
       &DeclarativeAudioBuffer::appendEffect,
       &DeclarativeAudioBuffer::countEffects,
       &DeclarativeAudioBuffer::atEffect,
       &DeclarativeAudioBuffer::clearEffects);
}

void DeclarativeAudioBuffer::appendEffect(
    QDeclarativeListProperty<DeclarativeAudioEffect> *list,
    DeclarativeAudioEffect *effect)
{
    DEBUG_POINT;
    DeclarativeAudioBuffer *_this =
        qobject_cast<DeclarativeAudioBuffer *>(list->object);
    if (!effect)
        return;

    if (!_this->m_effects.isEmpty()) {
       if (_this->m_effects.last()->m_effect) {
           _this->m_effects.last()->m_effect->linkTo(effect->m_effect);
       }
   }
   _this->m_effects.append(effect);
}

int DeclarativeAudioBuffer::countEffects(
    QDeclarativeListProperty<DeclarativeAudioEffect> *list)
{
    DEBUG_POINT;
    DeclarativeAudioBuffer *_this =
        qobject_cast<DeclarativeAudioBuffer *>(list->object);

    return _this->m_effects.count();
}

DeclarativeAudioEffect *DeclarativeAudioBuffer::atEffect(
    QDeclarativeListProperty<DeclarativeAudioEffect> *list, int index)
{
    DEBUG_POINT;
    DeclarativeAudioBuffer *_this =
        qobject_cast<DeclarativeAudioBuffer *>(list->object);
    return _this->m_effects.at(index);
}

void DeclarativeAudioBuffer::clearEffects(
    QDeclarativeListProperty<DeclarativeAudioEffect> *list)
{
    DEBUG_POINT;
    DeclarativeAudioBuffer *_this =
        qobject_cast<DeclarativeAudioBuffer *>(list->object);
    _this->m_effects.clear();
}

AudioBuffer *DeclarativeAudioBuffer::buffer()
{
    return m_buffer;
}

void DeclarativeAudioBuffer::play()
{
    DEBUG_POINT;
    playEx(m_lVolume, m_speed, m_loopCount);
}

void DeclarativeAudioBuffer::playEx(float volume, float speed, int loopCount)
{
    DEBUG_POINT;

    if (!m_buffer) {
        DEBUG_INFO("Sample not loaded!");
        return;
    }

    if (!m_mixer) {
        m_mixer = qobject_cast<DeclarativeAudioMixer *>(parent());
        if (!m_mixer)  {
            DEBUG_INFO("No mixer to play with!");
            return;
        }
    }

    // TODO: Add support for multiple instances
    m_playInstance = m_buffer->playWithMixer(m_mixer->m_audioMixer);
    if (m_playInstance.isNull()) {
        DEBUG_INFO("Failed to play sample" << m_source);
        return;
    }

    connect(m_playInstance, SIGNAL(finished()), this, SLOT(finished()));

    if (!m_effects.isEmpty())
        m_playInstance->setEffect(m_effects.first()->m_effect);
    setLoopCount(loopCount);
    setSpeed(speed);
    setLeftVolume(volume);
    setRightVolume(volume);
}

void DeclarativeAudioBuffer::finished()
{
    DEBUG_POINT;
    m_playInstance = NULL;
    emit playbackStopped();
}

void DeclarativeAudioBuffer::setLoopCount(int count)
{
    DEBUG_POINT;

    m_loopCount = count;
    if (!m_playInstance.isNull())
        m_playInstance->setLoopCount(count);

    emit loopCountChanged();
}

int DeclarativeAudioBuffer::loopCount() const
{
    DEBUG_POINT;
    return m_loopCount;
}

void DeclarativeAudioBuffer::setSpeed(float speed)
{
    DEBUG_INFO(speed);

    m_speed = speed;
    if (!m_playInstance.isNull())
        m_playInstance->setSpeed(speed);

    emit speedChanged();
}

float DeclarativeAudioBuffer::speed()
{
    DEBUG_POINT;
    return m_speed;
}

void DeclarativeAudioBuffer::setVolume(float volume)
{
    DEBUG_INFO(volume);

    setLeftVolume(volume);
    setRightVolume(volume);

    emit volumeChanged();
}

float DeclarativeAudioBuffer::volume() const
{
    DEBUG_POINT;
    return m_lVolume;
}

void DeclarativeAudioBuffer::setLeftVolume(float volume)
{
    DEBUG_INFO(volume);

    m_lVolume = volume;
    if (!m_playInstance.isNull())
        m_playInstance->setLeftVolume(volume);

    emit leftVolumeChanged();
}

float DeclarativeAudioBuffer::leftVolume() const
{
    DEBUG_POINT;
    return m_lVolume;
}

void DeclarativeAudioBuffer::setRightVolume(float volume)
{
    DEBUG_INFO(volume);

    m_rVolume = volume;
    if (!m_playInstance.isNull())
        m_playInstance->setRightVolume(volume);

    emit rightVolumeChanged();
}

float DeclarativeAudioBuffer::rightVolume() const
{
    DEBUG_POINT;
    return m_rVolume;
}

void DeclarativeAudioBuffer::seek(unsigned int samplePos)
{
    DEBUG_INFO(samplePos);
    if (!m_playInstance.isNull())
        m_playInstance->seek(samplePos);
}

unsigned int DeclarativeAudioBuffer::position()
{
    DEBUG_POINT;
    if (m_playInstance.isNull())
        return 0;
    return m_playInstance->position();
}

unsigned int DeclarativeAudioBuffer::length()
{
    DEBUG_POINT;
    if (!m_buffer)
        return 0;

    return m_buffer->getDataLength() / m_buffer->getBytesPerSample() /
        m_buffer->getNofChannels();
}

void DeclarativeAudioBuffer::stop()
{
    DEBUG_POINT;

    if (m_mixer && !m_playInstance.isNull()) {
        m_playInstance->stop();
        m_mixer->m_audioMixer.removeAudioSource(m_playInstance);
        emit playbackStopped();
    }

    m_playInstance = NULL;
}

bool DeclarativeAudioBuffer::playing()
{
    return m_playInstance ? true : false;
}
