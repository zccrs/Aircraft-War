/**
 * Copyright (c) 2011-2012 Nokia Corporation.
 * All rights reserved.
 *
 * Part of the Qt GameEnabler.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */

#include "pullaudioout.h"
#include <qglobal.h>
#include "trace.h" // For debug macros

#if defined(QTGAMEENABLER_USE_VOLUME_HACK) && defined(Q_OS_SYMBIAN)
    #include <sounddevice.h>
    #include <QSystemInfo>
    QTM_USE_NAMESPACE
#endif

using namespace GE;

/*!
  \class PullAudioOut
  \brief An object deploying QAudioOutput for sending the pre-mixed/processed
         audio data into an actual audio device. Uses QAudioOutput's pull mode.
*/

/*!
  Constructor.
*/
PullAudioOut::PullAudioOut(AudioSource *source, QObject *parent)
    : QIODevice(parent),
      m_source(source)
{
    DEBUG_INFO(this);

    QAudioFormat format;
    format.setFrequency(AUDIO_FREQUENCY);
    format.setChannels(AUDIO_CHANNELS);
    format.setSampleSize(AUDIO_SAMPLE_BITS);
    format.setCodec(GEDefaultAudioCodec);
    format.setByteOrder(GEByteOrder);
    format.setSampleType(GESampleType);

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());

    if (!info.isFormatSupported(format))
        format = info.nearestFormat(format);

    m_sendBufferSize = 4096;
    m_audioOutput = new QAudioOutput(info, format);

    open(QIODevice::ReadOnly | QIODevice::Unbuffered);
    connect(m_audioOutput, SIGNAL(stateChanged(QAudio::State)),
        SLOT(audioStateChanged(QAudio::State)));
    m_audioOutput->start(this);

#if defined(QTGAMEENABLER_USE_VOLUME_HACK) && defined(Q_OS_SYMBIAN)
    DEBUG_INFO("WARNING: Using the volume hack!");

    // This really ugly hack is used as the last resort. This allows us to
    // adjust the application volume in Symbian. The CMMFDevSound object lies
    // deep inside the QAudioOutput in Symbian implementation and it has the
    // needed functions. So, we get the needed object accessing it directly
    // from memory.
    unsigned int *pointer_to_abstract_audio =
            (unsigned int*)((unsigned char*)m_audioOutput + 8);

    unsigned int *dev_sound_wrapper = NULL;
    QSystemInfo sysInfo;
    QString mobilityVersion(sysInfo.version(QSystemInfo::QtMobility));

    if (mobilityVersion == QLatin1String("1.2.0") ||
        mobilityVersion == QLatin1String("1.2.1")) {
        dev_sound_wrapper = (unsigned int*)(*pointer_to_abstract_audio) + 16;
    } else if (mobilityVersion >= QLatin1String("1.2.2")) {
        // Do not use the volume hack with 1.2.2 and newer
        dev_sound_wrapper = NULL;
    } else {
        // Older than 1.2.0
        dev_sound_wrapper = (unsigned int*)(*pointer_to_abstract_audio) + 13;
    }

    if (dev_sound_wrapper) {
        unsigned int *temp = ((unsigned int*)(*dev_sound_wrapper) + 6);
        CMMFDevSound *devSound = (CMMFDevSound*)(*temp);
        devSound->SetVolume(devSound->MaxVolume() * 6 / 10);
    }
#endif
}

/*!
  Destructor.
*/
PullAudioOut::~PullAudioOut()
{
    m_audioOutput->stop();

    delete m_audioOutput;
    m_audioOutput = 0;

    close();
}

void PullAudioOut::audioStateChanged(QAudio::State state)
{
    Q_UNUSED(state);
    DEBUG_INFO("AudioStateChanged:" << state <<
        " error:" << m_audioOutput->error());

    if (m_audioOutput->error() == QAudio::UnderrunError)
        m_audioOutput->start(this);
}

/*!
  From QIODevice

  Returns the number of bytes that are available for reading.
*/
qint64 PullAudioOut::bytesAvailable() const
{
    return (qint64)m_sendBufferSize * sizeof(AUDIO_SAMPLE_TYPE);
}

/*!
  From QIODevice

  Writes up to maxSize bytes from data to the device. Since this is a ReadOnly
  device the implementation does nothing.
*/
qint64 PullAudioOut::writeData(const char *data, qint64 len)
{
    Q_UNUSED(data);
    Q_UNUSED(len);
    return 0;
}

/*!
  From QIODevice

  Reads up to maxSize bytes from the device into data.
*/
qint64 PullAudioOut::readData(char *data, qint64 maxlen)
{
    if (m_source.isNull()) {
        DEBUG_INFO("No audio source!");
        return -1;
    }

    int sampleCount = maxlen / 2;  // How many samples we mix maximumly.
    sampleCount /= 4;              // The partialbuffer upgrade part.

    if (sampleCount > m_sendBufferSize)
        sampleCount = m_sendBufferSize;

    // QAudioOutput does not use period size on Symbian, and periodSize
    // returns always the bufferSize.
#ifndef Q_OS_SYMBIAN
    // Use multiples of period size
    sampleCount = sampleCount * sizeof(AUDIO_SAMPLE_TYPE) /
        m_audioOutput->periodSize();
    if (sampleCount)
        sampleCount *= m_audioOutput->periodSize();
    else
        sampleCount = qMin(maxlen, (qint64)m_audioOutput->periodSize());
    sampleCount /= sizeof(AUDIO_SAMPLE_TYPE);
#endif // !Q_OS_SYMBIAN

    memset(data, 0, sampleCount * sizeof(AUDIO_SAMPLE_TYPE));
    int mixedSamples = m_source->pullAudio((AUDIO_SAMPLE_TYPE*)data,
        sampleCount);

    if (mixedSamples < sampleCount)
        mixedSamples = sampleCount;

    return (qint64)mixedSamples * sizeof(AUDIO_SAMPLE_TYPE);
}
