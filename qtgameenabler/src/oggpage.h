/**
 * Copyright (c) 2012 Nokia Corporation.
 * All rights reserved.
 *
 * Part of the Qt GameEnabler.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */

#ifndef GE_OGGPAGE_H
#define GE_OGGPAGE_H

#include <QtGlobal>

namespace GE {

#pragma pack(1)
class OggPage
{
public:
    OggPage();
    ~OggPage();

    void dump() const;

public:
    // OGG page header
    unsigned int m_capturePattern;
    unsigned char m_version;
    unsigned char m_headerType;
    quint64 m_granulePosition;
    unsigned int m_bitstreamSerialNum;
    unsigned int m_pageSequenceNum;
    unsigned int m_checksum;
    unsigned char m_pageSegments;
    unsigned char *m_segments;

    // Calculated stuff

    // Starting position of the page in the ogg file
    unsigned int m_pageStartPos;

    // Length of the page
    unsigned int m_pageLength;

    // Length of the page excluding the header
    unsigned int m_dataLength;

    // Starting position of the data in the ogg file
    unsigned int m_dataStartPos;

    OggPage *m_next;
    OggPage *m_prev;
};

#pragma pack()

} // namespace GE

#endif // GE_OGGPAGE_H
