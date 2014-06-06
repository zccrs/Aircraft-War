# Copyright (c) 2011-2012 Nokia Corporation.

# Comment the following line out for better performance. Using the definition
# enables debug logging which is convenient for locating problems in the code
# but is also very costly in terms of performance.
#DEFINES += GE_DEBUG

include($$PWD/qtgameenableraudio.pri)

INCLUDEPATH += $$PWD

HEADERS += $$PWD/src/gamewindow.h
SOURCES += $$PWD/src/gamewindow.cpp


symbian {
    LIBS += -lcone -leikcore -lavkon

    contains(DEFINES, GE_USE_OPENGL_ES_1_1) {
        LIBS += -llibEGL -llibgles_cm
    }
    else {
        LIBS += -llibEGL -llibGLESv2
    }

    # For HD output
    LIBS += -lws32 -laccmonitor

    HEADERS += $$PWD/src/hdmioutput.h
    SOURCES += $$PWD/src/hdmioutput.cpp
}

unix:!symbian {
    LIBS += -lX11

    contains(DEFINES, GE_USE_OPENGL_ES_1_1) {
        LIBS += -lEGL -lGLES_CM
    }
    else {
        LIBS += -lEGL -lGLESv2
    }

    maemo5 {
        # Maemo 5 specific
    }
    else {
        contains(MEEGO_EDITION, harmattan) {
            # Harmattan specific
            QT += meegographicssystemhelper
        }
        else {
            # Unix based desktop specific
            INCLUDEPATH += ../SDKPackage_OGLES2/Builds/OGLES2/Include
            LIBS += -L../SDKPackage_OGLES2/Builds/OGLES2/LinuxPC/Lib
        }
    }
}

windows {
    contains(DEFINES, GE_USE_OPENGL_ES_1_1) {
        message("GLES1_1 support undone for windows");
    }
    INCLUDEPATH += /PowerVRSDK/Builds/OGLES2/Include
    LIBS += -L/PowerVRSDK/Builds/OGLES2/WindowsPC/Lib

    LIBS += -llibEGL -llibGLESv2
}

message($$INCLUDEPATH)
message($$LIBS)

# End of file.
