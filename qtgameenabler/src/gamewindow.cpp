/**
 * Copyright (c) 2011-2012 Nokia Corporation.
 * All rights reserved.
 *
 * Part of the Qt GameEnabler.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */

#include <QtGui>

#ifdef Q_OS_LINUX
#include <QX11Info>
#endif

#ifdef MEEGO_EDITION_HARMATTAN
#include <qmeegographicssystemhelper.h>
#endif

#include "gamewindow.h"
#include "pushaudioout.h"
#include "pullaudioout.h"
#include "trace.h" // For debug macros

using namespace GE;

#ifndef GE_NOMOBILITY
QTM_USE_NAMESPACE
#endif


/*!
  \class GameWindow
  \brief QtWidget with native OpenGL ES 2.0 support. Replaces QGLWidget when
         real OpenGL ES is required instead of QtOpenGL.
*/


/*!
  Constructor.
*/
GameWindow::GameWindow(QWidget *parent)
    : QWidget(parent),
      m_prevTime(0),
      m_currentTime(0),
      m_frameTime(0.0f),
      m_fps(0.0f),
      m_paused(true),
      m_timerId(0),
      m_audioOutput(NULL),
      m_hdConnected(false)
#ifndef GE_NOMOBILITY
      ,m_systemDeviceInfo(NULL)
#endif
#ifdef Q_OS_SYMBIAN
    , m_hdmiOutput(NULL),
      m_volumeKeys(NULL)
#endif // Q_OS_SYMBIAN
{
    DEBUG_INFO(this);

    setAutoFillBackground(false);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_NativeWindow);
    setAttribute(Qt::WA_PaintOnScreen, true);
    setAttribute(Qt::WA_StyledBackground, false);
    setAttribute(Qt::WA_PaintUnclipped);

#ifdef GE_USE_MM_KEYS
    QApplication::setAttribute(Qt::AA_CaptureMultimediaKeys);
#endif // GE_USE_MM_KEYS

#ifdef MEEGO_EDITION_HARMATTAN
    QMeeGoGraphicsSystemHelper::setSwitchPolicy(QMeeGoGraphicsSystemHelper::NoSwitch);
#endif // MEEGO_EDITION_HARMATTAN

#ifdef Q_OS_SYMBIAN
    m_volumeKeys = new VolumeKeys(this, this);
#endif // Q_OS_SYMBIAN

#ifndef GE_NOMOBILITY
    m_systemDeviceInfo = new QSystemDeviceInfo(this);
    connect(m_systemDeviceInfo,
        SIGNAL(currentProfileChanged(QSystemDeviceInfo::Profile)),
        this, SLOT(profileChanged(QSystemDeviceInfo::Profile)));
#endif
}


/*!
  Destructor.
*/
GameWindow::~GameWindow()
{
    DEBUG_POINT;

    destroy();

#ifdef Q_OS_SYMBIAN
    delete m_hdmiOutput;
    m_hdmiOutput = NULL;

    delete m_volumeKeys;
    m_volumeKeys = NULL;
#endif // Q_OS_SYMBIAN

#ifndef GE_NOMOBILITY
    delete m_systemDeviceInfo;
#endif

    if (m_timerId)
        killTimer(m_timerId);

    stopAudio();
}


/*!
  Initializes OpenGL.
*/
void GameWindow::create()
{
    DEBUG_POINT;
    createEGL();

    onCreate();
    onInitEGL();

    m_currentTime = getTickCount();
    m_prevTime = m_currentTime;
    m_fps = 0.0f;
    m_frameTime = 0.0f;

    resume();

    DEBUG_INFO("Finished!");
}


/*!
  Called when the game window is to be destroyed.
*/
void GameWindow::destroy()
{
    DEBUG_POINT;

    onFreeEGL();

    eglMakeCurrent(eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroySurface(eglDisplay, eglSurface);

#ifdef Q_OS_SYMBIAN
    if (m_hdmiOutput)
        m_hdmiOutput->destroyWindow();
#endif // Q_OS_SYMBIAN

    onDestroy();
}


/*!
  Returns true if the current profile is silent.
*/
bool GameWindow::isProfileSilent() const
{
#ifndef GE_NOMOBILITY
    DEBUG_INFO("Current profile:" << m_systemDeviceInfo->currentProfile());

    return m_systemDeviceInfo->currentProfile() ==
        QSystemDeviceInfo::SilentProfile ? true : false;
#else
    return false;
#endif
}


/*!
  Returns the tick count in milliseconds.
*/
unsigned int GameWindow::getTickCount() const
{
    QTime now = QTime::currentTime();
    return (now.hour() * 3600 + now.minute() * 60 + now.second()) * 1000
            + now.msec();
}


/*!
  Pauses the execution.
*/
void GameWindow::pause()
{
    DEBUG_POINT;

    if (m_paused)
        return;

    m_paused = true;
    killTimer(m_timerId);
    stopAudio();
    m_timerId = 0;
    onPause();
}


/*!
  Resumes from the pause mode.
*/
void GameWindow::resume()
{
    DEBUG_POINT;

    if (!m_paused)
        return;

#ifdef Q_OS_SYMBIAN
    // Wait a little while to be able to see the new profile correctly.
    User::After( 500000 );
#endif // Q_OS_SYMBIAN

    m_paused = false;
    startAudio();
    onResume();
    m_timerId = startTimer(0);
}


/*!
  Starts the audio if not started.
*/
void GameWindow::startAudio()
{
    if (m_audioOutput || isProfileSilent())
        return;

    DEBUG_INFO("Starting audio..");
#ifdef GE_PULLMODEAUDIO
    m_audioOutput = new GE::PullAudioOut(&m_audioMixer, this);
#else
    m_audioOutput = new GE::PushAudioOut(&m_audioMixer, this);
#endif
}


/*!
  Stops the audio.
*/
void GameWindow::stopAudio()
{
    if (!m_audioOutput)
        return;

    delete m_audioOutput;
    m_audioOutput = NULL;
}

/*!
  Return true if the audio is enabled.
*/
bool GameWindow::audioEnabled() const
{
    return m_audioOutput ? true : false;
}

/*!
  Returns current audio mixer.
*/
AudioMixer &GameWindow::getMixer()
{
    return m_audioMixer;
}

/*!
  Returns true, if the HD output was and enabled and HDMI cable is connected.
*/
bool GameWindow::hdConnected() const
{
    return m_hdConnected;
}

/*!
  Returns current frame delta time.
*/
float GameWindow::getFrameTime() const
{
    return m_frameTime;
}

/*!
  Returns current FPS.
*/
float GameWindow::getFPS() const
{
    return m_fps;
}

/*!
  From QWidget.

  GameWindow does not use the paint event for drawing, but the application
  may use the paintevent for drawing onto the display when HD output is
  enabled.
*/
void GameWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
}


/*!
  Return native window handle
*/
EGLNativeWindowType GameWindow::getWindow()
{
    EGLNativeWindowType pwd;

#ifdef Q_OS_SYMBIAN
    if (hdConnected())
        pwd = (EGLNativeWindowType)m_hdmiOutput->hdmiWindow();
    else
        pwd = (EGLNativeWindowType)(winId()->DrawableWindow());
#else // !Q_OS_SYMBIAN
    pwd = (EGLNativeWindowType)winId();
#endif // Q_OS_SYMBIAN

    return pwd;
}

/*!
    Enable/disable HD output. Note that the HD output is not cloned into the
    device's display, so when the HDMI cable is connected nothing is shown on
    the device's display.
*/
void GameWindow::setHdOutput(bool onOff)
{
#ifdef Q_OS_SYMBIAN
    if (onOff) {
        if (!m_hdmiOutput) {
            m_hdmiOutput = new HdmiOutput(this, this);
            QT_TRAP_THROWING(m_hdmiOutput->startMonitoringL())
        }
    }
    else {
        if (m_hdConnected) {
            m_hdConnected = false;
            reinitEGL();
        }
        delete m_hdmiOutput;
        m_hdmiOutput = NULL;
    }
#else // !Q_OS_SYMBIAN
    Q_UNUSED(onOff);
#endif // Q_OS_SYMBIAN
}


/*!
  From QObject.

  Filters \a event sent or posted by \a object. If the event is
  ActivationChanged event, the pause mode is toggled. Otherwise the event is
  propagated.
*/
bool GameWindow::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::ActivationChange) {
        if (isActiveWindow())
            resume();
        else
            pause();

        DEBUG_INFO("ActivationChange event filtered -> toggle pause");
        return false;
    }
#ifdef GE_USE_MM_KEYS
    else if (event->type () == QEvent::KeyPress) {
        QKeyEvent* k = (QKeyEvent*)event;
        switch (k->key()) {
            case Qt::Key_VolumeDown:
                onVolumeDown();
                return true;
            case Qt::Key_VolumeUp:
                onVolumeUp();
                return true;
            default:
                break;
        }
    }
#endif // GE_USE_MM_KEYS

    // Let the event propagate for standard event processing.
    return QObject::eventFilter(object, event);
}

/*!
  From QWidget.

  Returns the width of the widget.
*/
int GameWindow::width()
{
#ifdef Q_OS_SYMBIAN
    if (m_hdConnected)
        return m_hdmiOutput->width();
#endif // Q_OS_SYMBIAN

    return QWidget::width();
}

/*!
  From QWidget.

  Returns the height of the widget.
*/
int GameWindow::height()
{
#ifdef Q_OS_SYMBIAN
    if (m_hdConnected)
        return m_hdmiOutput->height();
#endif // Q_OS_SYMBIAN

    return QWidget::height();
}

/*!
  From QWidget.

  Handles the resize events.
*/
void GameWindow::resizeEvent(QResizeEvent *event)
{
    int w, h;
    if (m_hdConnected) {
        // always fake hdmi output resolution
        w = width();
        h = height();
    }
    else {
        if (event->size().width() == event->oldSize().width() &&
            event->size().height() == event->oldSize().height()) {
            // The size has not changed.
            return;
        }

        w = event->size().width();
        h = event->size().height();
    }

    DEBUG_INFO("Setting the viewport. New size:" << w << "," << h);
    glViewport(0, 0, w, h);
    onSizeChanged(w, h);

    QWidget::resizeEvent(event);
}


/*!
  From QObject.

  Handles the timer events.
*/
void GameWindow::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    render();
}

/*!
  Called during startup, before OpenGL ES 2.0 is created, to let the
  application to allocate its resources.

  To be implemented in the derived class.
*/
void GameWindow::onCreate()
{
    DEBUG_POINT;
}

/*!
  Called after OpenGL ES 2.0 is created to let the application to allocate
  its EGL resources.

  To be implemented in the derived class.
*/
void GameWindow::onInitEGL()
{
    DEBUG_POINT;
}

/*!
  Called when EGL context is about to be freed to let the application free
  allocated EGL resources.

  To be implemented in the derived class.
*/
void GameWindow::onFreeEGL()
{
    DEBUG_POINT;
}

/*!
  Called when everything is about to be destroyed and the application is
  shutting down.

  To be implemented in the derived class.
*/
void GameWindow::onDestroy()
{
    DEBUG_POINT;
}


/*!
  Called when application needs to (re)render its screen.

  To be implemented in the derived class.
*/
void GameWindow::onRender()
{
    DEBUG_POINT;
}


/*!
  Called when the framework is going into pause mode.

  To be implemented in the derived class.
*/
void GameWindow::onPause()
{
    DEBUG_POINT;
}


/*!
  Called when the framework is resuming from the pause mode.

  To be implemented in the derived class.
*/
void GameWindow::onResume()
{
    DEBUG_POINT;
}


/*!
  Called when volume up -key is pressed.

  Default implementation increases absolute volume by 1/20.
*/
void GameWindow::onVolumeUp()
{
    DEBUG_POINT;
    m_audioMixer.setAbsoluteVolume(
        qMin(1.0f, m_audioMixer.absoluteVolume() + 1.0f / 20.0f));
}


/*!
  Called when volume down -key is pressed.

  Default implementation decreases absolute volume by 1/20.
*/
void GameWindow::onVolumeDown()
{
    DEBUG_POINT;
    m_audioMixer.setAbsoluteVolume(
        qMax(0.0f, m_audioMixer.absoluteVolume() - 1.0f / 20.0f));
}

/*!
  Called once before each frame, \a frameDelta attribute is set as the time
  between current frame and the previous one.

  To be implemented in the derived class.
*/
void GameWindow::onUpdate(const float frameDelta)
{
    Q_UNUSED(frameDelta);
}


/*!
  Called when the size of the screen has been changed. The application could
  update its projection, viewport and other size specific stuff here.

  To be implemented in the derived class.
*/
void GameWindow::onSizeChanged(int width, int height)
{
    Q_UNUSED(width);
    Q_UNUSED(height);
    DEBUG_POINT;
}


/*!
  Create and initialize objects required for OpenGL rendering
*/
void GameWindow::createEGL()
{
    DEBUG_POINT;

    // EGL variables
    eglDisplay	= 0;
    eglConfig	= 0;
    eglSurface	= 0;
    eglContext	= 0;

#ifdef Q_OS_LINUX
    eglDisplay =
        eglGetDisplay((EGLNativeDisplayType)x11Info().display());
#endif // Q_OS_LINUX

#ifdef Q_OS_WIN32
    HWND hwnd = this->winId();
    HDC dc = GetWindowDC(hwnd);
    eglDisplay = eglGetDisplay((EGLNativeDisplayType)dc);
#endif // Q_OS_WIN32

#ifdef Q_OS_SYMBIAN
    eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
#endif // Q_OS_SYMBIAN

    DEBUG_INFO("eglGetDisplay ==" << eglDisplay);

    EGLint majorVersion;
    EGLint minorVersion;

    if (!eglInitialize(eglDisplay, &majorVersion, &minorVersion)) {
        DEBUG_INFO("Error: eglInitialize() failed!");
        cleanupAndExit(eglDisplay);
    }

    DEBUG_INFO("eglInitialize() finished");

    EGLint pi32ConfigAttribs[13];
    pi32ConfigAttribs[0] = EGL_SURFACE_TYPE;
    pi32ConfigAttribs[1] = EGL_WINDOW_BIT;
    pi32ConfigAttribs[2] = EGL_RENDERABLE_TYPE;
    pi32ConfigAttribs[3] = EGL_OPENGL_ES2_BIT;

    pi32ConfigAttribs[4] = EGL_DEPTH_SIZE;
    pi32ConfigAttribs[5] = 8;
    pi32ConfigAttribs[6] = EGL_NONE;

    EGLint pi32ContextAttribs[3];
    pi32ContextAttribs[0] = EGL_CONTEXT_CLIENT_VERSION;
#ifndef GE_USE_OPENGL_ES_1_1
    pi32ContextAttribs[1] = 2;
#else
    pi32ContextAttribs[1] = 1;
#endif
    pi32ContextAttribs[2] = EGL_NONE;

    EGLint configs;

    if (!eglChooseConfig(eglDisplay, pi32ConfigAttribs, &eglConfig, 1, &configs)
            || (configs != 1))  {
        DEBUG_INFO("Error: eglChooseConfig() failed!");
        cleanupAndExit(eglDisplay);
    }

    DEBUG_INFO("eglChooseConfig() finished");

    eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, getWindow(), NULL);
    DEBUG_INFO("englCreateWindowSurface() finished");

    if (!testEGLError("eglCreateWindowSurface")) {
        cleanupAndExit(eglDisplay);
    }

    eglContext = eglCreateContext(eglDisplay, eglConfig, NULL,
                                  pi32ContextAttribs);
    DEBUG_INFO("eglCreateContext() finished");

    if (!testEGLError("eglCreateContext")) {
        cleanupAndExit(eglDisplay);
    }

    eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
    DEBUG_INFO("eglMakeCurrent() finished");

    if (!testEGLError("eglMakeCurrent")) {
        cleanupAndExit(eglDisplay);
    }
}


/*!
  Main run - method for the QtGameEnabler, processes a single frame and
  calls necesseary functions of the application using QtGE: update and
  onRender.
*/
void GameWindow::render()
{
    m_prevTime = m_currentTime;
    m_currentTime = getTickCount();
    m_frameTime = (float)(m_currentTime - m_prevTime) * 0.001f;

    if (m_frameTime != 0.0f) {
        m_fps = 1.0f / m_frameTime;
    }
    else {
        m_fps = 100000.0f;
    }

    // Check if a manual tick for audio is required.
    const bool manualTickRequired(m_audioOutput
                                  && m_audioOutput->needsManualTick());

    // 1st audio update
    if (manualTickRequired) {
        m_audioOutput->tick(); // Manual tick
    }

    onUpdate(m_frameTime);

    // 2nd audio update
    if (manualTickRequired) {
        m_audioOutput->tick(); // Manual tick
    }

    onRender();

    if (!eglSwapBuffers(eglDisplay, eglSurface)) {
        // eglSwapBuffers() failed!
        GLint errVal = eglGetError();

        /*
        EXPLANATION:

        http://library.forum.nokia.com/index.jsp?topic=/Nokia_Symbian3_Developers_Library/GUID-894AB487-C127-532D-852B-37CB0DEA1440.html

        On the Symbian platform, EGL handles the window resize in the next
        call to eglSwapBuffers(), which resizes the surface to match the new
        window size. If the preserve buffer option is in use, this function
        also copies across all the pixels from the old surface that overlap
        the new surface, although the exact details depend on the
        implementation.

        If the surface resize fails, eglSwapBuffers() returns EGL_FALSE and an
        EGL_BAD_ALLOC error is raised. This may mean that the implementation
        does not support the resizing of a surface or that there is not enough
        memory available (on a platform with GPU, this would be GPU rather
        than system memory).

        Applications must always monitor whether eglSwapBuffers() fails after
        a window resize. When it does fail, the application should do the
        following:
          - Call eglDestroySurface() to destroy the current EGL window surface.
          - Call eglCreateWindowSurface() to recreate the EGL window surface.

        This may cause a noticeable flicker and so should be done only when
        necessary.
        */

        DEBUG_INFO("eglSwapbuffers() failed with error:" << errVal);

        if (errVal == EGL_BAD_ALLOC || errVal == EGL_BAD_SURFACE) {
            if (errVal == EGL_BAD_ALLOC)
                DEBUG_INFO("Error was bad alloc, taking care of it.");

            eglDestroySurface(eglDisplay, eglSurface);

            eglSurface = eglCreateWindowSurface(eglDisplay,
                                                eglConfig,
                                                getWindow(),
                                                NULL);
            eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
        }
        else {
            cleanupAndExit(eglDisplay);
        }
    }
}


/*!
  Check for EGL errors. \a pszLocation should contain the last called EGL
  function and is used for DEBUG_INFO print. The method returns true in case of no
  errors, false otherwise.
*/
bool GameWindow::testEGLError(const char *pszLocation)
{
#ifndef GE_DEBUG
    Q_UNUSED(pszLocation);
#endif

    EGLint err = eglGetError();

    if (err != EGL_SUCCESS) {
        DEBUG_INFO(pszLocation << "failed" << err);
        return false;
    }

    return true;
}


/*!
  Cleans up the content of \a eglDisplay and exits.
*/
void GameWindow::cleanupAndExit(EGLDisplay eglDisplay)
{
    DEBUG_POINT;

    eglMakeCurrent(eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroySurface(eglDisplay, eglSurface);

#ifdef Q_OS_SYMBIAN
    if (m_hdmiOutput) {
        m_hdmiOutput->destroyWindow();
    }
#endif // Q_OS_SYMBIAN

    eglTerminate(eglDisplay);
    exit(0);
}

/*!
  Frees and creates the EGL context again. This is called when, for example,
  the underlaying native window changes.
*/
void GameWindow::reinitEGL()
{
    onFreeEGL();

    eglMakeCurrent(eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroySurface(eglDisplay, eglSurface);

#ifdef Q_OS_SYMBIAN
    if (m_hdmiOutput)
        m_hdmiOutput->destroyWindow();

    if (m_hdConnected)
        QT_TRAP_THROWING(m_hdmiOutput->createWindowL());
#endif // Q_OS_SYMBIAN

    createEGL();
    onInitEGL();
}

#ifndef GE_NOMOBILITY
/*!
  Called the active profile has changed. Audio output is stopped if the device
  has been set to silent profile.
*/
void GameWindow::profileChanged(QSystemDeviceInfo::Profile profile)
{
    if (profile == QSystemDeviceInfo::SilentProfile) {
        stopAudio();
    }
    else {
        if (!m_paused) {
            startAudio();
        }
    }
}
#endif

#ifdef Q_OS_SYMBIAN

/*!
  From IHdmiConnectionObserver.

  Called when HDMI cable has been connected or disconnected.
*/
void GameWindow::hdmiConnected(bool connected)
{
    DEBUG_POINT;
    m_hdConnected = connected;
    reinitEGL();
}

#endif // Q_OS_SYMBIAN




// !! GLHelpers !!
// Not actually part of the official GE, but helps a lot the actual programming.

#ifndef GE_USE_OPENGL_ES_1_1
/*!
  Compile shader program
*/
bool GameWindow::glhelpCompileShader(const QString &message, GLuint &shader)
{
    GLint status;
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    if(status == GL_FALSE) {
        DEBUG_INFO(message << "not compiled");

        int logLength, charsWritten;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        char* infoLog = new char(logLength);
        glGetShaderInfoLog(shader, logLength, &charsWritten, infoLog);
        DEBUG_INFO(infoLog);

        delete [] infoLog;
        return false;

    }
    else{

        DEBUG_INFO(message << "compiled");
        return false;
    }
}


/*!
  Link shader program
*/
bool GameWindow::glhelpLinkProgram(GLuint &program)
{

    GLint status;

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &status);

    if(status == GL_FALSE){

        DEBUG_INFO("programm not linked!");
        int logLength, charsWritten;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
        if(logLength > 0){
            char* infoLog = new char(logLength);

            glGetProgramInfoLog(program, logLength, &charsWritten, infoLog);

             DEBUG_INFO(infoLog);

            delete [] infoLog;
        }
        return false;
    }else{


        DEBUG_INFO("programm linked");
        return true;
    }
}


/*!
  Load vertex or fragment program code from file
*/
bool GameWindow::glhelpLoadProgram(const QString &source, GLuint &shader)
{
    GLint length;

    QFile file(source);
    if(file.exists()) {
        DEBUG_INFO( source << "found");
        file.open(QIODevice::ReadOnly | QIODevice::Text);

        const char * code = file.readAll();

        glShaderSource(shader, 1, &code, NULL);
        glGetShaderiv(shader, GL_SHADER_SOURCE_LENGTH, &length);

        DEBUG_INFO( "loaded" << length << "characters" );
        file.close();

        return true;
    }
    else {
        DEBUG_INFO( source << " not found");
        return false;
    }
}


/*!
  Swap matrix and set location
*/
void GameWindow::setUniformMatrix(int location, const QMatrix4x4 &matrix)
{
    // we need to flip rows and columns since QMatrix4x4
    // is defined in other-way-around than normal opengl
    float tempMat[4][4];
    for (int r=0; r<4; r++)
    for (int c=0; c<4; c++)
        tempMat[r][c] = matrix.constData()[ r*4 + c ];

    glUniformMatrix4fv(location, 1, GL_FALSE, (float*)&tempMat[0][0]);
}


/*!
  Create vertex and fragment shaders from provided source code. Create a new
  shader for them to be attached. Fills the references with the object handles.
*/
bool GameWindow::glhelpCreateShader(GLuint &vertexShader,
                                    GLuint &fragmentShader,
                                    GLuint &shaderProgram,
                                    const char *vertexShaderSrc,
                                    const char *fragmentShaderSrc)
{
    bool rval(true);
    GLint retval;

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, (const char**)&fragmentShaderSrc, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &retval);

    if (!retval) {
        DEBUG_INFO("Failed to compile the fragment shader!");
        rval = false;
    }

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, (const char**)&vertexShaderSrc, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &retval);

    if (!retval) {
        DEBUG_INFO("Failed to compile the vertex shader!");
        rval = false;
    }

    shaderProgram = glCreateProgram();

        // Attach the fragment and vertex shaders to it
    glAttachShader(shaderProgram, fragmentShader);
    glAttachShader(shaderProgram, vertexShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &retval);

    if (!retval) {
        DEBUG_INFO("Error linking the program!");
        rval = false;
    }

    return rval;
}

#endif

/*!
  Loads an image with QImage, converts it to GL's format, creates OpenGL
  texture and returns its handle.
*/
GLuint GameWindow::glhelpLoadTexture(QString imageFile)
{
    QImage image = QImage(imageFile);

    if (image.width() < 1 || image.height() < 1) {
        // Failed to load the texture
        return 0;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    GLuint *pTexData = new GLuint[image.width() * image.height()];
    GLuint *sdata = (GLuint*)image.bits();
    GLuint *tdata = pTexData;

    for (int y = 0; y < image.height(); y++) {
        for (int x = 0; x < image.width(); x++) {
            *tdata = ((*sdata&255) << 16) | (((*sdata>>8)&255) << 8)
                    | (((*sdata>>16)&255) << 0) | (((*sdata>>24)&255) << 24);

            sdata++;
            tdata++;
        }
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, pTexData);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    delete [] pTexData;
    return texture;
}
