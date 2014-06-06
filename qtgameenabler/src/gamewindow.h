/**
 * Copyright (c) 2011-2012 Nokia Corporation.
 * All rights reserved.
 *
 * Part of the Qt GameEnabler.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */

#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMatrix4x4>

#ifdef Q_WS_MAEMO_5
#include <QApplication>
#include <QMetaType>
#include <QtCore>
#include <QtGui>
#include <QTextStream>
#endif // Q_WS_MAEMO_5

#include <QWidget>
#ifndef GE_NOMOBILITY
#include <QSystemDeviceInfo>
#endif

#include <EGL/egl.h>
#ifdef GE_USE_OPENGL_ES_1_1
#include <GLES2/gl.h>
#else
#include <GLES2/gl2.h>
#endif

#include "geglobal.h"
#include "audiomixer.h"
#include "audioout.h"

#ifdef Q_OS_SYMBIAN
#include "hdmioutput.h"
#include "volumekeys.h"
#endif // Q_OS_SYMBIAN

#ifndef GE_NOMOBILITY
QTM_USE_NAMESPACE
#endif

namespace GE {

class Q_GE_EXPORT GameWindow : public QWidget
#ifdef Q_OS_SYMBIAN
                  ,public IVolumeKeyObserver
                  ,public IHdmiConnectionObserver
#endif // Q_OS_SYMBIAN
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = 0);
    virtual ~GameWindow();

public:
    void create();
    void destroy();

    bool isProfileSilent() const;
    bool audioEnabled() const;
    AudioMixer &getMixer();

    unsigned int getTickCount() const;
    float getFrameTime() const;
    float getFPS() const;

    void setHdOutput(bool onOff);
    bool hdConnected() const;

    int width();
    int height();

    // GL helper functions.
    GLuint glhelpLoadTexture(QString imageFile);

#ifndef GE_USE_OPENGL_ES_1_1
    bool glhelpCreateShader( GLuint &vertexShader,
                             GLuint &fragmentShader,
                             GLuint &shaderProgram,
                             const char *vertexShaderSrc,
                             const char *fragmentShaderSrc );

    // program manipulation, Originally named as setLocation -> note.
    void setUniformMatrix(int location, const QMatrix4x4 &matrix);

    // code reading, loading, compiling and linking
    bool glhelpLoadProgram(const QString &source, GLuint &shader);
    bool glhelpCompileShader(const QString &message, GLuint &shader);
    bool glhelpLinkProgram(GLuint &program);
#endif

public slots:
    void pause();
    void resume();
    void startAudio();
    void stopAudio();

protected: // Application callbacks, override these in your own derived class
    virtual void onCreate();
    virtual void onInitEGL();
    virtual void onFreeEGL();
    virtual void onDestroy();
    virtual void onRender();
    virtual void onPause();
    virtual void onResume();
    virtual void onUpdate(const float frameDelta);
    virtual void onSizeChanged(int width, int height);
    virtual void onVolumeUp();
    virtual void onVolumeDown();

protected: // From QWidget (and other base classes)
    bool eventFilter(QObject *object, QEvent *event);
    void paintEvent(QPaintEvent *event);
    virtual void resizeEvent(QResizeEvent *event);
    void timerEvent(QTimerEvent *event);

private slots:
#ifndef GE_NOMOBILITY
    void profileChanged(QSystemDeviceInfo::Profile profile);
#endif

private: // For internal functionality
    void createEGL();
    void reinitEGL();
    void render();
    bool testEGLError(const char* pszLocation);
    void cleanupAndExit(EGLDisplay eglDisplay);
    virtual EGLNativeWindowType getWindow();

#ifdef Q_OS_SYMBIAN
private: // From IHdmiConnectionObserver
    void hdmiConnected(bool connected);
#endif

protected: // Data
    // EGL variables
    EGLDisplay eglDisplay;
    EGLConfig eglConfig;
    EGLSurface eglSurface;
    EGLContext eglContext;

private: // Data
    // Time calculation
    unsigned int m_prevTime;
    unsigned int m_currentTime;
    float m_frameTime;
    float m_fps;
    bool m_paused;
    int m_timerId;

    // Audio
    AudioOut *m_audioOutput;
    AudioMixer m_audioMixer;

    // HD output
    bool m_hdConnected;

#ifndef GE_NOMOBILITY
    // For current profile
    QSystemDeviceInfo *m_systemDeviceInfo;
#endif

#ifdef Q_OS_SYMBIAN
    HdmiOutput *m_hdmiOutput;
    VolumeKeys *m_volumeKeys;
#endif // Q_OS_SYMBIAN

};

} // namespace GE

#endif // GAMEWINDOW_H
