# Copyright (c) 2011-2012 Nokia Corporation.

# Comment the following line out for better performance. Using the definition
# enables debug logging which is convenient for locating problems in the code
# but is also very costly in terms of performance.
#DEFINES += GE_DEBUG

# Support longer audio buffers than 2^20 samples per channel
DEFINES += QTGAMEENABLER_SUPPORT_LONG_SAMPLES

# For enabling Ogg Vorbis support
DEFINES += QTGAMEENABLER_SUPPORT_OGG_VORBIS

# STB's pullmode API is not needed
DEFINES += STB_VORBIS_NO_PULLDATA_API

INCLUDEPATH += $$PWD/src

HEADERS  += \
    $$PWD/src/trace.h \
    $$PWD/src/geglobal.h \
    $$PWD/src/audiobuffer.h \
    $$PWD/src/audiobufferplayinstance.h \
    $$PWD/src/audiomixer.h \
    $$PWD/src/audioout.h \
    $$PWD/src/pushaudioout.h \
    $$PWD/src/pullaudioout.h \
    $$PWD/src/audiosourceif.h \
    $$PWD/src/audioeffect.h \
    $$PWD/src/echoeffect.h \
    $$PWD/src/cutoffeffect.h

SOURCES += \
    $$PWD/src/audiobuffer.cpp \
    $$PWD/src/audiobufferplayinstance.cpp \
    $$PWD/src/audiomixer.cpp \
    $$PWD/src/pushaudioout.cpp \
    $$PWD/src/pullaudioout.cpp \
    $$PWD/src/audiosourceif.cpp \
    $$PWD/src/audioeffect.cpp \
    $$PWD/src/echoeffect.cpp \
    $$PWD/src/cutoffeffect.cpp

contains(DEFINES, QTGAMEENABLER_SUPPORT_OGG_VORBIS) {
    HEADERS += \
        $$PWD/src/stb_vorbis.h \
        $$PWD/src/vorbisdecoder.h \
        $$PWD/src/oggpage.h \
        $$PWD/src/oggconstants.h \
        $$PWD/src/vorbissource.h

    SOURCES += \
        $$PWD/src/stb_vorbis.c \
        $$PWD/src/vorbisdecoder.cpp \
        $$PWD/src/oggpage.cpp \
        $$PWD/src/vorbissource.cpp
}

QT += core gui

symbian :{
    message(Symbian build)

    CONFIG += mobility
    MOBILITY += multimedia systeminfo

    # For volume keys
    LIBS += -lremconcoreapi -lremconinterfacebase

    # Uncomment the following define to enable a very ugly hack to set the
    # volume level on Symbian devices higher. By default, on Symbian, the volume
    # level is very low when audio is played using QAudioOutput class. For now,
    # this ugly hack is the only way to set the volume louder.
    #
    # WARNING: The volume hack (see the GEAudioOut.cpp file) is VERY DANGEROUS
    # because the data to access the volume interface is retrieved manually with
    # pointer manipulation. Should the library, in which the interface is
    # implemented, be modified even a tiny bit, the application using this hack
    # might crash.
    #
    #DEFINES += QTGAMEENABLER_USE_VOLUME_HACK

    contains(DEFINES, QTGAMEENABLER_USE_VOLUME_HACK) {
        # Include paths and libraries required for the volume hack.
        message(Symbian volume hack enabled)
        INCLUDEPATH += /epoc32/include/mmf/common
        INCLUDEPATH += /epoc32/include/mmf/server
        LIBS += -lmmfdevsound
    }

    #DEFINES += GE_PULLMODEAUDIO
    contains(S60_VERSION, 5.0){
        INCLUDEPATH += /epoc32/include/middleware
    }else{
        INCLUDEPATH += /epoc32/include/mw
    }
    HEADERS += $$PWD/src/volumekeys.h
    SOURCES += $$PWD/src/volumekeys.cpp
}

unix:!symbian {
    maemo5 {
        message(Maemo 5 build)

        QT += multimedia
        CONFIG += mobility
        MOBILITY += systeminfo
    } else {
        contains(MEEGO_EDITION, harmattan) {
            message(Harmattan build)

            DEFINES += MEEGO_EDITION_HARMATTAN
            CONFIG += mobility
            MOBILITY += multimedia systeminfo
        }
        else {
            message(Unix based desktop build)

            CONFIG += mobility
            MOBILITY += multimedia systeminfo
            #DEFINES += GE_PULLMODEAUDIO
        }
    }
}

windows {
    message(Windows desktop build)

    QT += multimedia
    DEFINES += GE_NOMOBILITY
}

# End of file.
