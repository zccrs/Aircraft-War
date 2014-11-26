# Add more folders to ship with the application, here
TARGET = AircraftWar
# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

QT += network webkit
VERSION = 2.1.0
CONFIG += mobility
MOBILITY += systeminfo

sound.source = sound
sound.target = ./
symbian {
    DEPLOYMENTFOLDERS += sound
    TARGET.UID3 = 0x20072840
    TARGET.CAPABILITY += NetworkServices
    contains(S60_VERSION, 5.0){
        DEFINES += Q_OS_SYMBIAN_V5
        INCLUDEPATH += $$[QT_INSTALL_PREFIX]/epoc32/include/middleware
        INCLUDEPATH += $$[QT_INSTALL_PREFIX]/include/Qt
    }
}
contains(MEEGO_EDITION, harmattan){
    icon64.files += $${TARGET}64.png
    icon64.path = /usr/share/icons/hicolor/64x64/apps
    data.files += ./data/MeeGo.png
    data.files += fzmw.ttf
    data.files += ./sound/*.wav
    data.path = /opt/$${TARGET}/data
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

    INSTALLS += iconsvg icon64 gameclassify  data
}


win32{
    RESOURCES += pcdata.qrc font.qrc
    HEADERS += \
        src/mypos.h

    SOURCES += \
        src/mypos.cpp
}

simulator{
    RESOURCES += font.qrc
    DEPLOYMENTFOLDERS += sound
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
HEADERS += \
    src/bullet.h \
    src/enemy.h \
    src/myplanes.h \
    src/mythread.h \
    src/prop.h \
    src/settings.h \
    src/windowplanes.h \
    src/utility.h \
    src/mynetworkaccessmanagerfactory.h \
    src/myimage.h
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
    src/mynetworkaccessmanagerfactory.cpp \
    src/myimage.cpp

# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()
include(qtgameenabler/qtgameenableraudioplugin.pri)

OTHER_FILES += \
    qml/meego/Music.qml \
    qml/meego/main.qml \
    qml/general/UserCenter.qml \
    qml/general/Setting.qml \
    qml/general/Rank.qml \
    qml/general/HttpRequest.qml \
    qml/general/Game.qml \
    qml/general/DeviceInfo.qml \
    qml/general/About.qml \
    qml/symbian3/Music.qml \
    qml/symbian3/main.qml \
    i18n/des.js \
    i18n/comment.css \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/manifest.aegis \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog \
    qtc_packaging/debian_harmattan/AircraftWar.conf \
    AircraftWarmeego.svg

RESOURCES += des.qrc qml.qrc planes.qrc
