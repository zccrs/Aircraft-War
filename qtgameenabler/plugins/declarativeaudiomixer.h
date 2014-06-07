/**
 * Copyright (c) 2011-2012 Nokia Corporation.
 * All rights reserved.
 *
 * Part of the Qt GameEnabler.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */

#ifndef DECLARATIVEAUDIOMIXER_H
#define DECLARATIVEAUDIOMIXER_H

#include <QDeclarativeItem>
#ifndef GE_NOMOBILITY
#ifndef Q_OS_UNIX
#include <QSystemDeviceInfo>
#endif
#endif

#include "audiomixer.h"
#include "audioout.h"
#include "audiosourceif.h"
#include "audiobufferplayinstance.h"
#include "audiobuffer.h"

#ifdef Q_OS_SYMBIAN
#include "volumekeys.h"
#endif // Q_OS_SYMBIAN

#ifndef GE_NOMOBILITY
#ifndef Q_OS_UNIX
QTM_USE_NAMESPACE
#endif
#endif

namespace GE {

class DeclarativeAudioBuffer;
class DeclarativeAudioEffect;

class DeclarativeAudioMixer : public QDeclarativeItem
#ifdef Q_OS_SYMBIAN
                             ,public IVolumeKeyObserver
#endif // Q_OS_SYMBIAN
{
    Q_OBJECT
    Q_PROPERTY(float absoluteVolume READ absoluteVolume WRITE setAbsoluteVolume )
    Q_PROPERTY(float generalVolume READ generalVolume WRITE setGeneralVolume)
    Q_PROPERTY(bool enabled READ audioEnabled WRITE enableAudio)
    Q_PROPERTY(QDeclarativeListProperty<GE::DeclarativeAudioEffect> effect READ effectProperty DESIGNABLE false)

public:
    explicit DeclarativeAudioMixer(QDeclarativeItem *parent = 0);
    virtual ~DeclarativeAudioMixer();

public slots:
    float absoluteVolume() const;
    void setAbsoluteVolume(float volume);

    float generalVolume();
    void setGeneralVolume(float volume);

    void stopPlayback();

    bool audioEnabled() const;
    void enableAudio(bool enabled);

    QDeclarativeListProperty<DeclarativeAudioEffect> effectProperty();

protected slots:
#ifndef GE_NOMOBILITY
#ifndef Q_OS_UNIX
    void profileChanged(QSystemDeviceInfo::Profile profile);
#endif
#endif

#ifdef Q_OS_SYMBIAN
private:
    virtual void onVolumeUp();
    virtual void onVolumeDown();
#endif // Q_OS_SYMBIAN

private:
    void timerEvent(QTimerEvent *event);
    void startAudio();
    void stopAudio();
    static void appendEffect(
        QDeclarativeListProperty<DeclarativeAudioEffect> *list,
        DeclarativeAudioEffect *effect);
    static int countEffects(
        QDeclarativeListProperty<DeclarativeAudioEffect> *list);
    static DeclarativeAudioEffect *atEffect(
        QDeclarativeListProperty<DeclarativeAudioEffect> *list, int index);
    static void clearEffects(
        QDeclarativeListProperty<DeclarativeAudioEffect> *list);
signals:
    void addVolum();
    void subVolum();
    //void absoluteVolumeChanged();
private:
    AudioOut *m_audioOutput;
    AudioMixer m_audioMixer;
    int m_timerId;
    bool m_audioEnabled;
    QList<DeclarativeAudioEffect *> m_effects;

#ifndef GE_NOMOBILITY
#ifndef Q_OS_UNIX
    QSystemDeviceInfo *m_systemDeviceInfo;
#endif
#endif

#ifdef Q_OS_SYMBIAN
    VolumeKeys *m_volumeKeys;
#endif // Q_OS_SYMBIAN

    friend class DeclarativeAudioBuffer;
};

} // namespace GE

QML_DECLARE_TYPE(GE::DeclarativeAudioMixer)

#endif // DECLARATIVEAUDIOMIXER_H
