/**
 * Copyright (c) 2011-2012 Nokia Corporation.
 * All rights reserved.
 *
 * Part of the Qt GameEnabler.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */

#ifndef VOLUMEKEYS_H
#define VOLUMEKEYS_H

#include <QtGui>
#include <remconcoreapitargetobserver.h>
#include <remconcoreapitarget.h>
#include <remconinterfaceselector.h>
#include <QTimer>

namespace GE {

class IVolumeKeyObserver
{
public:
    virtual void onVolumeUp() = 0;
    virtual void onVolumeDown() = 0;
};

class VolumeKeys : public QObject,
                   public MRemConCoreApiTargetObserver
{
    Q_OBJECT

public:
    explicit VolumeKeys(QObject *parent, IVolumeKeyObserver *observer);
    ~VolumeKeys();

private: // from MRemConCoreApiTargetObserver
    void MrccatoCommand(TRemConCoreApiOperationId operationId,
        TRemConCoreApiButtonAction buttonAct);
    QTimer timer_add,timer_sub,timer_temp_add,timer_temp_sub;
signals:
    void addVolum();
    void subVolum();
private:
    CRemConInterfaceSelector *m_interfaceSelector;
    CRemConCoreApiTarget *m_coreTarget;
    IVolumeKeyObserver *m_observer;
};

} // namespace GE

#endif // VOLUMEKEYS_H
