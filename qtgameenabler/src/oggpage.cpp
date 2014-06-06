/**
 * Copyright (c) 2012 Nokia Corporation.
 * All rights reserved.
 *
 * Part of the Qt GameEnabler.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */

#include <memory.h>
#include <stdlib.h>

#include "trace.h"
#include "oggpage.h"
#include "oggconstants.h"

using namespace GE;

OggPage::OggPage()
{
    memset(this, 0, sizeof(OggPage));
}

OggPage::~OggPage()
{
    free(m_segments);
}

void OggPage::dump() const
{
    DEBUG_INFO("***");
    DEBUG_INFO("Page" << this);
    DEBUG_INFO("    capturePattern:" << m_capturePattern);
    DEBUG_INFO("           version:" << m_version);
    DEBUG_INFO("        headerType:" << m_headerType << "(" <<
        (m_headerType & OggContinuation ? " Continuation " : "") <<
        (m_headerType & OggBOS ? " BOS " : "") <<
        (m_headerType & OggEOS ? " EOS " : "") << ")"
     );
    DEBUG_INFO("   granulePosition:" << m_granulePosition);
    DEBUG_INFO("bitstreamSerialNum:" << m_bitstreamSerialNum);
    DEBUG_INFO("   pageSequenceNum:" << m_pageSequenceNum);
    DEBUG_INFO("          checksum:" << m_checksum);
    DEBUG_INFO("      pageSegments:" << m_pageSegments);
    DEBUG_INFO("      pageStartPos:" << m_pageStartPos);
    DEBUG_INFO("        dataLength:" << m_dataLength);
    DEBUG_INFO("      dataStartPos:" << m_dataStartPos);

    if (!m_segments) {
        DEBUG_INFO("No segments");
        return;
    }

    for (int t = 0; t < m_pageSegments; t++) {
        DEBUG_INFO("          Segments:" << t << "," << m_segments[t]);
    }
}
