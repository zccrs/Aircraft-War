# Copyright (c) 2011-2012 Nokia Corporation.

QT += declarative

include($$PWD/qtgameenableraudio.pri)

INCLUDEPATH += $$PWD/src
INCLUDEPATH += $$PWD/plugins

HEADERS  += \
    $$PWD/plugins/declarativeaudiomixer.h \
    $$PWD/plugins/declarativeaudiobuffer.h \
    $$PWD/plugins/declarativeaudioeffect.h \
    $$PWD/plugins/declarativeechoeffect.h \
    $$PWD/plugins/declarativecutoffeffect.h \
    $$PWD/plugins/audioplugin.h

SOURCES += \
    $$PWD/plugins/declarativeaudiomixer.cpp \
    $$PWD/plugins/declarativeaudiobuffer.cpp \
    $$PWD/plugins/declarativeaudioeffect.cpp \
    $$PWD/plugins/declarativeechoeffect.cpp \
    $$PWD/plugins/declarativecutoffeffect.cpp \
    $$PWD/plugins/audioplugin.cpp

# End of file.
