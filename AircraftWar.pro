# Add more folders to ship with the application, here
QT += network webkit multimedia
TARGET = AircraftWar
TEMPLATE = app
VERSION = 2.0.1
#CONFIG += mobility
MOBILITY += systeminfo

general_qml.source = qml/general
general_qml.target = qml

all_qml.source = qml
all_qml.target = ./

symbian3_qml.source = qml/symbian3
symbian3_qml.target = qml

meego_qml.source = qml/meego
meego_qml.target = qml

folder_02.source = data
folder_02.target = ./

include(qtgameenabler/qtgameenableraudioplugin.pri)

unix{
    RESOURCES += font.qrc
    #OTHER_FILES += ./qml/general/*
}
win32{
    RESOURCES += font.qrc
    #OTHER_FILES += ./qml/general/*
}
# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH += ./

symbian {
    #DEPLOYMENTFOLDERS += general_qml
    #DEPLOYMENTFOLDERS += symbian3_qml
    TARGET.UID3 = 0x20072840
    TARGET.CAPABILITY += NetworkServices
    contains(S60_VERSION, 5.0){
        DEFINES += Q_OS_SYMBIAN_V5
        INCLUDEPATH += $$[QT_INSTALL_PREFIX]/epoc32/include/middleware
        INCLUDEPATH += $$[QT_INSTALL_PREFIX]/include/Qt
    }
}
contains(MEEGO_EDITION, harmattan){
    #DEPLOYMENTFOLDERS += meego_qml general_qml
    RESOURCES += font.qrc

    icon64.files += $${TARGET}64.png
    icon64.path = /usr/share/icons/hicolor/64x64/apps
    data.files += ./data/MeeGo.png
    data.files += ./data/fzmw.ttf
    data.path =/opt/$${TARGET}/data
    iconsvg.files += $${TARGET}meego.svg
    iconsvg.path = /usr/share/themes/base/meegotouch/$${TARGET}
    # Classify the application as a game to support volume keys on Harmattan.
    gameclassify.files += qtc_packaging/debian_harmattan/$${TARGET}.conf
    gameclassify.path = /usr/share/policy/etc/syspart.conf.d

    export(icon64.files)
    export(icon64.path)
    export(gameclassify.files)
    export(gameclassify.path)
    export(iconsvg.files)
    export(iconsvg.path)
    export(data.files)
    export(data.path)

    INSTALLS += icon64 gameclassify iconsvg data
}
simulator:{
    #DEPLOYMENTFOLDERS += all_qml general_qml
}
# Smart Installer package's UID
# This UID is from the protected range and therefore the package will
# fail to install if self-signed. By default qmake uses the unprotected
# range value if unprotected UID is defined for the application and
# 0x2002CCCF value if protected UID is given to the application
#symbian:DEPLOYMENT.installer_header = 0x2002CCCF

# Allow network access on Symbian

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=

# Speed up launching on MeeGo/Harmattan when using applauncherd daemon
CONFIG += qdeclarative-boostable

# Add dependency to Symbian components
# CONFIG += qt-components

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    src/bullet.cpp \
    src/enemy.cpp \
    src/myplanes.cpp \
    src/mythread.cpp \
    src/prop.cpp \
    src/settings.cpp \
    src/windowplanes.cpp \
    src/utility.cpp \
    src/mynetworkaccessmanagerfactory.cpp


unix{
    SOURCES += \
    src/mypos.cpp
}
win32{
    SOURCES += \
    src/mypos.cpp
}
# Please do not modify the following two lines. Required for deployment.
include(myqmlapplicationviewer/myqmlapplicationviewer.pri)
qtcAddDeployment()

OTHER_FILES += \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/manifest.aegis \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog \
    qml/general/* \
    qml/meego/* \
    qml/symbian3/* \
    i18n/des.js \
    qml/general/DeviceInfo.qml

RESOURCES += \
    planes.qrc \
    music.qrc \
    sound.qrc \
    des.qrc \
    qml.qrc

HEADERS += \
    src/bullet.h \
    src/enemy.h \
    src/myplanes.h \
    src/mythread.h \
    src/prop.h \
    src/settings.h \
    src/windowplanes.h \
    src/utility.h \
    src/mynetworkaccessmanagerfactory.h


unix{
    HEADERS += \
    src/mypos.h
}
win32{
    HEADERS += \
    src/mypos.h
}
