/**
 * Copyright (c) 2011-2012 Nokia Corporation.
 * All rights reserved.
 *
 * Part of the Qt GameEnabler.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */

#ifndef DECLARATIVEAUDIOBUFFER_H
#define DECLARATIVEAUDIOBUFFER_H

#include <QDeclarativeItem>
#include "declarativeaudiomixer.h"

namespace GE {

class DeclarativeAudioEffect;

class DeclarativeAudioBuffer : public QDeclarativeItem
{
    Q_OBJECT
    Q_PROPERTY(QString source READ source WRITE setSource)
    Q_PROPERTY(GE::DeclarativeAudioMixer *mixer READ mixer WRITE setMixer)
    Q_PROPERTY(QDeclarativeListProperty<GE::DeclarativeAudioEffect> effect
        READ effectProperty DESIGNABLE false)
    Q_PROPERTY(float volume READ volume WRITE setVolume
        NOTIFY volumeChanged)
    Q_PROPERTY(float leftVolume READ leftVolume WRITE setLeftVolume
        NOTIFY leftVolumeChanged)
    Q_PROPERTY(float rightVolume READ rightVolume WRITE setRightVolume
        NOTIFY rightVolumeChanged)
    Q_PROPERTY(float speed READ speed WRITE setSpeed
        NOTIFY speedChanged)
    Q_PROPERTY(float loopCount READ loopCount WRITE setLoopCount
        NOTIFY loopCountChanged)
    Q_PROPERTY(float position READ position WRITE seek)

public:
    explicit DeclarativeAudioBuffer(QDeclarativeItem *parent = 0);
    virtual ~DeclarativeAudioBuffer();

public:
    QString source() const;
    void setSource(QString source);

    DeclarativeAudioMixer *mixer() const;
    void setMixer(DeclarativeAudioMixer *mixer);

    DeclarativeAudioEffect *effect() const;
    void setEffect(DeclarativeAudioEffect *effect);

    QDeclarativeListProperty<DeclarativeAudioEffect> effectProperty();

    AudioBuffer *buffer();

    void setLoopCount(int count);
    int loopCount() const;

    void setSpeed(float speed);
    float speed();

    void setVolume(float volume);
    float volume() const;

    void setLeftVolume(float volume);
    float leftVolume() const;

    void setRightVolume(float volume);
    float rightVolume() const;

    void seek(unsigned int samplePos);
    unsigned int position();

public slots:
    void play();
    void playEx(float volume, float speed, int loopCount = 0);
    void stop();
    bool playing();
    unsigned int length();

signals:
    void loopCountChanged();
    void volumeChanged();
    void leftVolumeChanged();
    void rightVolumeChanged();
    void speedChanged();
    void playbackStopped();

private slots:
    void finished();

private:
    static void appendEffect(
        QDeclarativeListProperty<DeclarativeAudioEffect> *list,
        DeclarativeAudioEffect *effect);
    static int countEffects(
        QDeclarativeListProperty<DeclarativeAudioEffect> *list);
    static DeclarativeAudioEffect *atEffect(
        QDeclarativeListProperty<DeclarativeAudioEffect> *list, int index);
    static void clearEffects(
        QDeclarativeListProperty<DeclarativeAudioEffect> *list);

private:
    DeclarativeAudioMixer *m_mixer;
    QList<DeclarativeAudioEffect *> m_effects;
    AudioBuffer *m_buffer;
    QPointer<AudioBufferPlayInstance> m_playInstance;
    QString m_source;
    int m_loopCount;
    float m_speed;
    float m_lVolume;
    float m_rVolume;
};

} // namespace GE

QML_DECLARE_TYPE(GE::DeclarativeAudioBuffer)

#endif // DECLARATIVEAUDIOBUFFER_H
