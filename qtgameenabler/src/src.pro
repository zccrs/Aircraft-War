# Copyright (c) 2011-2012 Nokia Corporation.

TEMPLATE = lib
TARGET = QtGameEnabler$${QT_LIBINFIX}
MOC_DIR = .moc
OBJECTS_DIR = .obj
DESTDIR = .lib
CONFIG += create_prl
DEFINES += GE_BUILD_LIB

contains(QT_CONFIG, reduce_exports): CONFIG += hide_symbols

GE_PATH = ../
include(../qtgameenabler.pri)

PUBLIC_HEADERS = $$HEADERS
HEADERS += $$PRIVATE_HEADERS

feature.path = $$[QT_INSTALL_DATA]/mkspecs/features
feature.files = ../features/qtgameenabler.prf
INSTALLS += feature

win32 {
    DLLDESTDIR = ../bin
    !static:DEFINES += QT_MAKEDLL

    CONFIG(debug, debug|release) {
        TARGET = $$member(TARGET, 0)d
    }
}

symbian {
    DEFINES += QT_MAKEDLL
    CONFIG += epocallowdlldata
    contains(QT_EDITION, OpenSource) {
        TARGET.CAPABILITY = LocalServices ReadUserData UserEnvironment WriteUserData
    } else {
        TARGET.CAPABILITY = All -Tcb
    }

    load(data_caging_paths)

    isEmpty(QT_LIBINFIX) {
        # TODO: signed UID
        TARGET.UID3 = 0xE005B322
    } else {
        TARGET.UID3 = 0xE005B322
    }

    qtgameenablerlibraries.sources = QtGameEnabler$${QT_LIBINFIX}.dll
    qtgameenablerlibraries.path = /sys/bin
    DEPLOYMENT += qtgameenablerlibraries
} else {
    target.path += $$[QT_INSTALL_LIBS]
    INSTALLS += target

    install_headers.path = $$[QT_INSTALL_HEADERS]/$$TARGET
    install_headers.files = $$PUBLIC_HEADERS
    INSTALLS += install_headers
}

# End of file.
