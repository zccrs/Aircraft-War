/**
 * Copyright (c) 2011-2012 Nokia Corporation.
 * All rights reserved.
 *
 * Part of the Qt GameEnabler.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */

#include "audioplugin.h"
#include "declarativeaudiomixer.h"
#include "declarativeaudiobuffer.h"
#include "declarativeaudioeffect.h"
#include "declarativeechoeffect.h"
#include "declarativecutoffeffect.h"

using namespace GE;

AudioPlugin::AudioPlugin(QObject *parent) :
    QDeclarativeExtensionPlugin(parent)
{
}

void AudioPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(QLatin1String(uri) == QLatin1String("GEAudio"));

    qmlRegisterType<GE::DeclarativeAudioMixer>(uri, 1, 0, "AudioMixer");
    qmlRegisterType<GE::DeclarativeAudioBuffer>(uri, 1, 0, "AudioBuffer");
    qmlRegisterType<GE::DeclarativeAudioEffect>(uri, 1, 0, "AudioEffect");
    qmlRegisterType<GE::DeclarativeEchoEffect>(uri, 1, 0, "EchoEffect");
    qmlRegisterType<GE::DeclarativeCutOffEffect>(uri, 1, 0, "CutOffEffect");
}

#ifdef GE_BUILD_LIB
Q_EXPORT_PLUGIN2(AudioPlugin, AudioPlugin)
#endif // GE_BUILD_LIB
