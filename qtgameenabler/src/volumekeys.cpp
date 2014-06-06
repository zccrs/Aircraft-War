/**
 * Copyright (c) 2011-2012 Nokia Corporation.
 * All rights reserved.
 *
 * Part of the Qt GameEnabler.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */

#include "volumekeys.h"
#include "trace.h"

using namespace GE;

/*!
  \class VolumeKeys
  \brief Monitors the state of volume up and down buttons on Symbian devices.
*/


/*!
  Constructor.
*/
VolumeKeys::VolumeKeys(QObject *parent, IVolumeKeyObserver *observer) :
    QObject(parent),
    m_interfaceSelector(NULL),
    m_coreTarget(NULL),
    m_observer(observer)
{
    DEBUG_INFO(this);

    timer_add.setInterval(50);
    timer_sub.setInterval(50);
    connect(&timer_add,SIGNAL(timeout()),SIGNAL(addVolum()));
    connect(&timer_sub,SIGNAL(timeout()),SIGNAL(subVolum()));

    timer_temp_add.setSingleShot(true);
    timer_temp_sub.setSingleShot(true);
    connect(&timer_temp_add,SIGNAL(timeout()),&timer_add,SLOT(start()));
    connect(&timer_temp_sub,SIGNAL(timeout()),&timer_sub,SLOT(start()));
    QT_TRAP_THROWING(
        m_interfaceSelector = CRemConInterfaceSelector::NewL();
        m_coreTarget = CRemConCoreApiTarget::NewL(*m_interfaceSelector, *this);
        m_interfaceSelector->OpenTargetL();
    );

}

/*!
  Destructor.
*/
VolumeKeys::~VolumeKeys()
{
    DEBUG_POINT;

    delete m_interfaceSelector;
    m_interfaceSelector = NULL;
    m_coreTarget = NULL; // owned by interfaceselector
}

/*!
  From MRemConCoreApiTargetObserver.

  Checks if volume up/down keys are pressed and calls observer's (GameWindow's)
  callbacks respectively.
*/
void VolumeKeys::MrccatoCommand(TRemConCoreApiOperationId operationId,
                                TRemConCoreApiButtonAction buttonAct)
{
    DEBUG_INFO("operation:" << operationId << " action:" << buttonAct);

    if (operationId == ERemConCoreApiVolumeUp)
    {
        if (buttonAct == ERemConCoreApiButtonClick)
        {
            emit addVolum();
            m_observer->onVolumeUp();
            qDebug()<<"VolumeUp pressed";
        }
        else if(buttonAct== ERemConCoreApiButtonPress)
        {
            timer_temp_add.start(100);
        }
        else if(buttonAct==ERemConCoreApiButtonRelease)
        {
            if(timer_add.isActive())
                timer_add.stop();
            if(timer_temp_add.isActive())
                timer_temp_add.stop();
        }
    }
    else if (operationId == ERemConCoreApiVolumeDown)
    {
        if (buttonAct == ERemConCoreApiButtonClick)
        {
            emit subVolum();
            m_observer->onVolumeDown();
            qDebug()<<"VolumeDown pressed";
        }
        else if(buttonAct== ERemConCoreApiButtonPress)
        {
            timer_temp_sub.start(100);
        }
        else if(buttonAct==ERemConCoreApiButtonRelease)
        {
            if(timer_sub.isActive())
                timer_sub.stop();
            if(timer_temp_sub.isActive())
                timer_temp_sub.stop();
        }
    }
}
