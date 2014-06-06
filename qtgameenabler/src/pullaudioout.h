/**
 * Copyright (c) 2011-2012 Nokia Corporation.
 * All rights reserved.
 *
 * Part of the Qt GameEnabler.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */

#ifndef GEPULLAUDIOOUT_H
#define GEPULLAUDIOOUT_H

#include <QAudioOutput>
#include <QIODevice>
#include <QPointer>
#include "geglobal.h"
#include "audioout.h"

namespace GE {

/**
 *
 * Pull  mode solution
 *
 */
class Q_GE_EXPORT PullAudioOut : public QIODevice,
                     public AudioOut
{
    Q_OBJECT

public:
    PullAudioOut(AudioSource *source, QObject *parent = 0);
    virtual ~PullAudioOut();

public: // from QIODevice
    qint64 bytesAvailable() const;
    qint64 readData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 len);

public slots:
    void audioStateChanged(QAudio::State state);

protected:
    QAudioOutput* m_audioOutput;        // Owned
    int m_sendBufferSize;
    QPointer<AudioSource> m_source; // Not owned
};

} // namespace GE

#endif // GEPULLAUDIOOUT_H
