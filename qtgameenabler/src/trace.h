/**
 * Copyright (c) 2011-2012 Nokia Corporation.
 * All rights reserved.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */

#ifndef TRACE_H
#define TRACE_H

#ifdef GE_DEBUG
    #include <QDebug>

    #define DEBUG_POINT qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__
    #define DEBUG_INFO(ARG...) \
        qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << ":" << ARG
#else
    #define DEBUG_POINT do {} while (0)
    #define DEBUG_INFO(string) do {} while (0)
#endif

#endif // TRACE_H

// End of file.
