TARGET = harbour-aircraftwar

QT += network webkit quick
VERSION = 2.1.0

DEFINES += Q_OS_SAILFISH

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
    src/myimage.h \
    src/myhttprequest.h
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
    src/myimage.cpp \
    src/myhttprequest.cpp

CONFIG += sailfishapp
RESOURCES += qml_sailfish.qrc planes.qrc
OTHER_FILES += \
        rpm/harbour-aircraftwar.changes.in \
        rpm/harbour-aircraftwar.spec \
        rpm/harbour-aircraftwar.yaml \
        translations/*.ts \
        harbour-aircraftwar.desktop \
        i18n/comment.css \

data.files += fzmw.ttf
data.files += ./sound/*.wav
data.path = /usr/share/$${TARGET}/data

INSTALLS += data

include (src/cryptolib/cryptolib.pri)

