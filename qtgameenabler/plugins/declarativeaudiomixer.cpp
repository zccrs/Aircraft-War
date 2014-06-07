/**
 * Copyright (c) 2011-2012 Nokia Corporation.
 * All rights reserved.
 *
 * Part of the Qt GameEnabler.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */

#include "declarativeaudiomixer.h"
#include "declarativeaudioeffect.h"
#include "pushaudioout.h"
#include "pullaudioout.h"
#include "trace.h"

using namespace GE;

DeclarativeAudioMixer::DeclarativeAudioMixer(QDeclarativeItem *parent)
    : QDeclarativeItem(parent),
      m_audioOutput(NULL),
      m_timerId(0),
      m_audioEnabled(true)
#ifndef GE_NOMOBILITY
#ifndef Q_OS_UNIX
      ,m_systemDeviceInfo(NULL)
#endif
#endif
#ifdef Q_OS_SYMBIAN
      ,m_volumeKeys(NULL)
#endif // Q_OS_SYMBIAN
{
    DEBUG_INFO(this);

#ifndef GE_NOMOBILITY
#ifndef Q_OS_UNIX
    m_systemDeviceInfo = new QSystemDeviceInfo(this);
    connect(m_systemDeviceInfo,
        SIGNAL(currentProfileChanged(QSystemDeviceInfo::Profile)),
        this, SLOT(profileChanged(QSystemDeviceInfo::Profile)));
#endif
#endif

#ifdef Q_OS_SYMBIAN
    m_volumeKeys = new VolumeKeys(this, this);
    connect(m_volumeKeys,SIGNAL(addVolum()),SIGNAL(addVolum()));
    connect(m_volumeKeys,SIGNAL(subVolum()),SIGNAL(subVolum()));
#endif // Q_OS_SYMBIAN

    startAudio();
}

DeclarativeAudioMixer::~DeclarativeAudioMixer()
{
    DEBUG_POINT;
    stopAudio();
#ifndef GE_NOMOBILITY
#ifndef Q_OS_UNIX
    delete m_systemDeviceInfo;
#endif
#endif
#ifdef Q_OS_SYMBIAN
    delete m_volumeKeys;
#endif // Q_OS_SYMBIAN
}

void DeclarativeAudioMixer::startAudio()
{
    DEBUG_POINT;

    if (m_audioOutput || !m_audioEnabled
#ifndef GE_NOMOBILITY
#ifndef Q_OS_UNIX
        || m_systemDeviceInfo->currentProfile() ==
        QSystemDeviceInfo::SilentProfile
#endif
#endif
        )
    {
        return;
    }

    DEBUG_INFO("Starting audio..");
#ifdef GE_PULLMODEAUDIO
    m_audioOutput = new GE::PullAudioOut(&m_audioMixer, this);
#else // !GE_PULLMODEAUDIO
    m_audioOutput = new GE::PushAudioOut(&m_audioMixer, this);
#endif // GE_PULLMODEAUDIO

    if (m_audioOutput->needsManualTick())
        m_timerId = startTimer(5);
}

void DeclarativeAudioMixer::stopAudio()
{
    DEBUG_POINT;

    if (!m_audioOutput)
        return;

    DEBUG_INFO(m_audioOutput);

    delete m_audioOutput;
    m_audioOutput = 0;

    if (m_timerId) {
        killTimer(m_timerId);
        m_timerId = 0;
    }
}

void DeclarativeAudioMixer::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    if (m_audioOutput)
        m_audioOutput->tick();
}

void DeclarativeAudioMixer::enableAudio(bool enable)
{
    DEBUG_POINT;

    m_audioEnabled = enable;

    if (m_audioEnabled)
        startAudio();
    else
        stopAudio();
}

float DeclarativeAudioMixer::absoluteVolume() const
{
    DEBUG_POINT;
    return m_audioMixer.absoluteVolume();
}

void DeclarativeAudioMixer::setAbsoluteVolume(float volume)
{
    DEBUG_POINT;
    m_audioMixer.setAbsoluteVolume(volume);
}

float DeclarativeAudioMixer::generalVolume()
{
    DEBUG_POINT;
    return m_audioMixer.generalVolume();
}

void DeclarativeAudioMixer::setGeneralVolume(float volume)
{
    DEBUG_POINT;
    m_audioMixer.setGeneralVolume(volume);
}

bool DeclarativeAudioMixer::audioEnabled() const
{
    DEBUG_POINT;
    return m_audioOutput ? true : false;
}

#ifdef Q_OS_SYMBIAN
void DeclarativeAudioMixer::onVolumeUp()
{
    DEBUG_POINT;
    m_audioMixer.setAbsoluteVolume(
        qMin(1.0f, m_audioMixer.absoluteVolume() + 1.0f / 20.0f));
}

void DeclarativeAudioMixer::onVolumeDown()
{
    DEBUG_POINT;
    m_audioMixer.setAbsoluteVolume(
        qMax(0.0f, m_audioMixer.absoluteVolume() - 1.0f / 20.0f));
}
#endif // Q_OS_SYMBIAN

void DeclarativeAudioMixer::stopPlayback()
{
    DEBUG_POINT;
    m_audioMixer.destroyList();
}

#ifndef GE_NOMOBILITY
#ifndef Q_OS_UNIX
void DeclarativeAudioMixer::profileChanged(QSystemDeviceInfo::Profile profile)
{
    if (profile == QSystemDeviceInfo::SilentProfile) {
        stopAudio();
    } else {
        if (m_audioEnabled)
            startAudio();
    }
}
#endif
#endif

QDeclarativeListProperty<DeclarativeAudioEffect>
    DeclarativeAudioMixer::effectProperty()
{
    return QDeclarativeListProperty<DeclarativeAudioEffect>(this, &m_effects,
       &DeclarativeAudioMixer::appendEffect,
       &DeclarativeAudioMixer::countEffects,
       &DeclarativeAudioMixer::atEffect,
       &DeclarativeAudioMixer::clearEffects);
}

void DeclarativeAudioMixer::appendEffect(
    QDeclarativeListProperty<DeclarativeAudioEffect> *list,
    DeclarativeAudioEffect *effect)
{
    DEBUG_POINT;
    DeclarativeAudioMixer *_this =
        qobject_cast<DeclarativeAudioMixer *>(list->object);
    if (!effect)
        return;

    if (!_this->m_effects.isEmpty()) {
       if (_this->m_effects.last()->m_effect) {
           _this->m_effects.last()->m_effect->linkTo(effect->m_effect);
       }
   }

   _this->m_effects.append(effect);
   _this->m_audioMixer.setEffect(_this->m_effects.first()->m_effect);
}

int DeclarativeAudioMixer::countEffects(
    QDeclarativeListProperty<DeclarativeAudioEffect> *list)
{
    DEBUG_POINT;
    DeclarativeAudioMixer *_this =
        qobject_cast<DeclarativeAudioMixer *>(list->object);

    return _this->m_effects.count();
}

DeclarativeAudioEffect *DeclarativeAudioMixer::atEffect(
    QDeclarativeListProperty<DeclarativeAudioEffect> *list, int index)
{
    DEBUG_POINT;
    DeclarativeAudioMixer *_this =
        qobject_cast<DeclarativeAudioMixer *>(list->object);
    return _this->m_effects.at(index);
}

void DeclarativeAudioMixer::clearEffects(
    QDeclarativeListProperty<DeclarativeAudioEffect> *list)
{
    DEBUG_POINT;
    DeclarativeAudioMixer *_this =
        qobject_cast<DeclarativeAudioMixer *>(list->object);
    _this->m_effects.clear();
    _this->m_audioMixer.setEffect(NULL);
}

