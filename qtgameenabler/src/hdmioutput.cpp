/**
 * Copyright (c) 2011-2012 Nokia Corporation.
 * All rights reserved.
 *
 * Part of the Qt GameEnabler.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */

#include "hdmioutput.h"
#include "trace.h"

_LIT(KWgName, "HdmiOutput");
const TUint32 KWgId = 0xeeeeeeee; // TODO: define unique id
const TUint32 KHdScreen = 1;

using namespace GE;

/*!
  \class HdmiOutput
  \brief Monitors the state of HDMI, and creates a new full-screen window for
         HD output if the HDMI cable is connected.
*/


/*!
  Constructor.
*/
HdmiOutput::HdmiOutput(QObject *parent,
    IHdmiConnectionObserver *observer) :
    QObject(parent),
    m_observer(observer),
    m_accMonitor(NULL),
    m_screenDevice(NULL),
    m_windowGroup(NULL),
    m_window(NULL)
{
    DEBUG_INFO(this);
}

/*!
  Destructor.
*/
HdmiOutput::~HdmiOutput()
{
    DEBUG_POINT;

    delete m_accMonitor;
    destroyWindow();
}

/*!
  Creates the accessory monitor for HDMI.
*/
void HdmiOutput::startMonitoringL()
{
    DEBUG_POINT;

    m_accMonitor = CAccMonitor::NewL();

    RConnectedAccessories array;
    CleanupClosePushL(array);
    m_accMonitor->GetConnectedAccessoriesL(array);

    for (int i = 0; i < array.Count(); i++) {
        if (array[i]->Exists(KAccMonHDMI))
            ConnectedL(array[i]);
    }

    CleanupStack::PopAndDestroy(&array);

    RAccMonCapabilityArray capabilityArray;
    CleanupClosePushL(capabilityArray);
    capabilityArray.Append(KAccMonHDMI);
    m_accMonitor->StartObservingL(this, capabilityArray);
    CleanupStack::PopAndDestroy(&capabilityArray);
}

/*!
  Creates a full-screen window on screen device 1 (HDMI output device).
*/
void HdmiOutput::createWindowL()
{
    DEBUG_POINT;

    if (m_window)
        return;

    m_wsSession.Connect();

    m_screenDevice = new (ELeave) CWsScreenDevice(m_wsSession);
    User::LeaveIfError(m_screenDevice->Construct(KHdScreen));

    m_wsSession.ComputeMode(RWsSession::EPriorityControlDisabled);

    m_windowGroup = new (ELeave) RWindowGroup(m_wsSession);
    User::LeaveIfError(m_windowGroup->Construct(KWgId, m_screenDevice));
    User::LeaveIfError(m_windowGroup->SetName(KWgName));

    m_windowGroup->EnableReceiptOfFocus(EFalse);

    TSize size = m_screenDevice->SizeInPixels();

    m_window = new (ELeave) RWindow(m_wsSession);
    User::LeaveIfError(m_window->Construct(*m_windowGroup, (TUint32)m_window));

    m_window->SetExtent(TPoint(0, 0), size);
    m_window->SetOrdinalPosition(0, ECoeWinPriorityAlwaysAtFront + 1);
    m_window->SetNonFading(ETrue);
    m_window->SetVisible(ETrue);
    m_window->Activate();
}

/*!
  From MAccMonitorObserver

  Called when an accessory device is connected.
*/
void HdmiOutput::ConnectedL(CAccMonitorInfo *accessoryInfo)
{
    DEBUG_POINT;

    if (accessoryInfo->Exists(KAccMonHDMI)) {
        if (m_observer)
            m_observer->hdmiConnected(true);
    }
}

/*!
  From MAccMonitorObserver

  Called when an accessory device is disconnected.
*/
void HdmiOutput::DisconnectedL(CAccMonitorInfo *accessoryInfo)
{
    DEBUG_POINT;

    if (accessoryInfo->Exists(KAccMonHDMI)) {
        if (m_observer)
            m_observer->hdmiConnected(false);
    }
}

/*!
  From MAccMonitorObserver

  Called when an error occured. Intentionally empty, since hdmiConnected
  callback should not leave.
*/
void HdmiOutput::AccMonitorObserverError(TInt error)
{
    DEBUG_INFO("AccMonitorObserverError:" << error);
}

/*!
  Destroys the HD output window.
*/
void HdmiOutput::destroyWindow()
{
    DEBUG_POINT;

    if (!m_window)
        return;

    m_window->Close();
    delete m_window;
    m_window = NULL;

    m_windowGroup->Close();
    delete m_windowGroup;
    m_windowGroup = NULL;

    m_wsSession.Close();
}

/*!
  Getter for the HD output window.
*/
RWindow *HdmiOutput::hdmiWindow()
{
    DEBUG_POINT;
    return m_window;
}

/*!
  Return the HD output window width, or zero if the window is not created.
*/
int HdmiOutput::width()
{
    DEBUG_POINT;
    return m_screenDevice ? m_screenDevice->SizeInPixels().iWidth : 0;
}

/*!
  Return the HD output window height, or zero if the window is not created.
*/
int HdmiOutput::height()
{
    DEBUG_POINT;
    return m_screenDevice ? m_screenDevice->SizeInPixels().iHeight : 0;
}
