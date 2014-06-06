/**
 * Copyright (c) 2011-2012 Nokia Corporation.
 * All rights reserved.
 *
 * Part of the Qt GameEnabler.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */

#ifndef AUDIOPLUGIN_H
#define AUDIOPLUGIN_H

#include <QDeclarativeExtensionPlugin>
#include "geglobal.h"

namespace GE {

class Q_GE_EXPORT AudioPlugin : public QDeclarativeExtensionPlugin
{
    Q_OBJECT

public:
    explicit AudioPlugin(QObject *parent = 0);

    void registerTypes(const char *uri);
};

} // namespace GE

#endif // AUDIOPLUGIN_H
