/**
 * Copyright (c) 2012 Nokia Corporation.
 * All rights reserved.
 *
 * Part of the Qt GameEnabler.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */

#ifndef OGGCONSTANTS_H
#define OGGCONSTANTS_H

namespace GE {

// SggO = OggS
const unsigned int OggCapturePatternMagic = 0x5367674F;

// See Vorbis I specification
// Xiph.Org Foundation
// August 11, 2011

// 'The first packet on this page is a continuation of the previous packet
// in the logical bitstream.'
const unsigned char OggContinuation = 0x01;

// 'Beginning Of Stream. This page is the first page in the logical
// bitstream. The BOS flag must be set on the first page of every logical
// bitstream, and must not be set on any other page.'
const unsigned char OggBOS = 0x02;

// 'End Of Stream. This page is the last page in the logical bitstream. The EOS
// flag must be set on the final page of every logical bitstream, and must not
// be set on any other page.'
const unsigned char OggEOS = 0x04;

// Length of the OGG header from capture pattern to page segments
const unsigned int OggPageHeaderLen = 27;

// 'The first Vorbis packet (the identification header), which uniquely
// identifies a stream as Vorbis audio, is placed alone in the first page of the
// logical Ogg stream. This results in a first Ogg page of exactly 58 bytes at
// the very beginning of the logical stream.'
const unsigned int OggFirstPageLen = 58;

}

#endif // OGGCONSTANTS_H
