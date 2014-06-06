# Copyright (c) 2011-2012 Nokia Corporation.

TEMPLATE = lib
CONFIG += plugin
TARGET = $$qtLibraryTarget(GEAudio)
TARGETPATH = GEAudio
MOC_DIR = .moc
OBJECTS_DIR = .obj
DESTDIR = .lib

contains(QT_CONFIG, reduce_exports): CONFIG += hide_symbols

GE_PATH = ../
DEFINES += GE_BUILD_LIB
include(../qtgameenableraudioplugin.pri)

target.path = $$[QT_INSTALL_IMPORTS]/$$TARGETPATH

qmldir.files += $$PWD/qmldir
qmldir.path +=  $$[QT_INSTALL_IMPORTS]/$$TARGETPATH

INSTALLS += target qmldir

symbian {
    CONFIG += epocallowdlldata
}

# End of file.
