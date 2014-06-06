/**
 * Copyright (c) 2011-2012 Nokia Corporation.
 * All rights reserved.
 *
 * Part of the Qt GameEnabler.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */

#include <QtGlobal>
#include <stdlib.h>
#include <memory.h>

#include "trace.h"
#include "vorbisdecoder.h"
#include "oggpage.h"
#include "oggconstants.h"

using namespace GE;

const int maxblocksize = 512 * 1024;

/*!
  Constructor. If cached is set to true the decoder loads the entire encoded
  file into memory.
*/
VorbisDecoder::VorbisDecoder(bool cached, QObject *parent) :
    QObject(parent),
    m_file(this),
    m_pageList(0),
    m_pos(0),
    m_cached(cached),
    m_cache(NULL),
    m_vorbis(NULL),
    m_decSamples(NULL),
    m_decSamplesLen(0),
    m_maxDecSamplesLen(0),
    m_readData(NULL),
    m_decodedLength(0)
{
}

/*!
  Destructor.
*/
VorbisDecoder::~VorbisDecoder()
{
    m_file.close();

    OggPage *p = m_pageList;
    while (p) {
        OggPage *pn = p->m_next;
        delete p;
        p = pn;
    }

    vorbisUninit();
    delete m_cache;
}

/*!
  Loads the given file, scans all the pages and initializes stb vorbis.
  Returns true if successful.
*/
bool VorbisDecoder::load(QString &filename)
{
    m_file.setFileName(filename);

    if (!m_file.open(QIODevice::ReadOnly)) {
        DEBUG_INFO("Failed to open file" << filename);
        return false;
    }

    if (m_cached) {
        m_length = m_file.size();
        m_pos = 0;
        m_cache = (unsigned char*)malloc(m_length);
        qint64 len = m_file.read((char*)m_cache, m_length);
        if (len != m_length) {
            DEBUG_INFO("Failed to read file" << filename << "read"
                <<  len << "expected" << m_length);
            return false;
        }
        m_file.close();
    }

    if (!scan())
        return false;

	if (vorbisInit() < 0) {
	    DEBUG_INFO("Failed to initialize vorbis decoder");
	    return false;
    }

    return true;
}

/*!
  Scans all ogg pages of the file in order to support seeking into arbitrary
  positions. Returns true if successful.
*/
bool VorbisDecoder::scan()
{
    // Read the first page. It must be the beginning-of-stream page.
    m_pageList = readPageHeader();
    if (!m_pageList || !m_pageList->m_headerType & OggBOS) {
        DEBUG_INFO("Failed to read BOS page");
        delete m_pageList;
        m_pageList = NULL;
        return false;
    }
    OggPage *page = m_pageList;

    // Read all the pages until eof or end-of-stream page reachead
    while (!(page->m_headerType & OggEOS) &&
            (page->m_next = readPageHeader()))
    {
        page->m_next->m_prev = page;
        page = page->m_next;
        //page->dump();
        seek(tell() + page->m_dataLength);
    }

    m_decodedLength = page->m_granulePosition;
    // TODO: fill this properly (the stream does not always start from 0)
    m_granuleOffset = 0;

    return true;
}

/*!
  Seek for an ogg page which contains (not really true, read forward) given
  PCM sample. samplePos is in units of PCM samples per channel. If vorbis
  packets span over multiple pages, the returned page is the last page. You'll
  have to seek backwards from the returned page in order to find the position
  where you should start decoding. Returns NULL if page was not found.
*/
const OggPage *VorbisDecoder::seekPage(quint64 samplePos)
{
    OggPage *p = m_pageList;
    if (!p)
        return NULL;

    do {
        if (p->m_granulePosition &&
            p->m_granulePosition - m_granuleOffset > samplePos) {
            return p;
        }
    } while ((p = p->m_next));

    return NULL;
}

/*!
  Returns the first decodable audio page. The beginning of an ogg file contains
  headers pages which are skipped.
*/
const OggPage *VorbisDecoder::firstAudioPage()
{
    OggPage *page = m_pageList;
    // 'The granule position of these first pages containing only headers is zero.'
    while (page->m_granulePosition == 0) {
        page = page->m_next;
        if (!page)
            return NULL;
    }
    return page;
}

/*!
  Returns info about the loaded file, such as channels and sample rate.
*/
const stb_vorbis_info *VorbisDecoder::fileInfo()
{
    return &m_info;
}

/*!
  Reads the header of an ogg page from current file position and returns
  filled instance of an OggPage. Returns NULL if something is wrong with the
  page.
*/
OggPage *VorbisDecoder::readPageHeader()
{
    int len;

    if (!m_file.isOpen() && !m_cache)
        return NULL;

    OggPage *page = new OggPage();
    if (!page) {
        DEBUG_INFO("Failed to allocate a page");
        return NULL;
    }

    page->m_pageStartPos = tell();

    len = read((unsigned char*)page, OggPageHeaderLen, -1);
    if (!len) {
        DEBUG_INFO("Failed to read a page");
        delete page;
        return NULL;
    }

    if (page->m_capturePattern != OggCapturePatternMagic) {
        DEBUG_INFO("Capture pattern mismatch");
        delete page;
        return NULL;
    }

    // First page does not contain segments at all. It contains only an Vorbis
    // identification header.
    if (page->m_headerType & OggBOS) {
        seek(OggFirstPageLen);
        page->m_segments = NULL;
        page->m_dataLength = 0;
        page->m_pageLength = OggFirstPageLen;
    } else {
        page->m_segments = (unsigned char*)malloc(page->m_pageSegments);
        if (!page->m_segments) {
            DEBUG_INFO("Failed to allocate page segments");
            delete page;
            return NULL;
        }
        len = read(page->m_segments, page->m_pageSegments, -1);
        if (len != page->m_pageSegments) {
            DEBUG_INFO("Read" << len << "segment lengths, expected"
                << page->m_pageSegments);
            delete page;
            return NULL;
        }

        for (int t = 0; t < page->m_pageSegments; t++) {
            page->m_dataLength += page->m_segments[t];
        }
    }
    page->m_dataStartPos = tell();
    page->m_pageLength = page->m_dataStartPos - page->m_pageStartPos +
        page->m_dataLength;

    return page;
}

int VorbisDecoder::read(unsigned char *buf, int len, int pos)
{
    if (m_cached) {
        if (!m_cache)
            return -1;
        if (pos >=0)
            seek(pos);
        int l = qMin(len, m_length - m_pos);
        memcpy(buf, &m_cache[m_pos], l);
        m_pos += l;
        return l;
    } else {
        if (!m_file.isOpen())
            return -1;
        if (pos >= 0) {
            if (!seek(pos))
                return -1;
        }
        return m_file.read((char*)buf, len);
    }
}

bool VorbisDecoder::seek(int pos)
{
    if (m_cached) {
        if (!m_cache)
            return false;
        if (pos >= m_length || pos < 0)
            return false;
        m_pos = pos;
        return m_pos;
    } else {
        if (!m_file.isOpen())
            return false;
        return m_file.seek(pos);
    }
}

int VorbisDecoder::tell()
{
    if (m_cached) {
        if (!m_cache)
            return -1;
        return m_pos;
    } else {
        if (!m_file.isOpen())
            return -1;
        return m_file.pos();
    }
}

/*!
  Reads the header pages from the beginning of loaded ogg file.
*/
unsigned char *VorbisDecoder::readHeaderPages(int *len)
{
    OggPage *page = m_pageList;
    if (!page)
        return NULL;

    *len = 0;
    // 'The granule position of these first pages containing only headers is zero.'
    while (page->m_granulePosition == 0) {
        *len += page->m_pageLength;
        page = page->m_next;
        if (!page)
            return NULL;
    }

    unsigned char *data = (unsigned char*)malloc(*len);
    if (!data)
        return NULL;

    int rlen = read(data, *len, 0);
    if (rlen != *len) {
        DEBUG_INFO("Read" << rlen << "bytes, expected" << *len);
        free(data);
        return NULL;
    }

    return data;
}

/*!
  Decode the entire file into memory.
*/
unsigned char *VorbisDecoder::decodeAll(unsigned int *len)
{
    int allocLen = m_decodedLength * sizeof(short) * m_info.channels;
    if (!m_vorbis)
        return NULL;

    unsigned char *outBuf = (unsigned char*)malloc(allocLen);
    if (!outBuf) {
        DEBUG_INFO("Failed to allocate" << allocLen << "bytes!");
        return NULL;
    }
    *len = 0;

    m_readPos = firstAudioPage()->m_pageStartPos;
    int rlen = 0;
    while ((rlen = read(m_readData, 65536 * 2, m_readPos)) > 0) {
        int leftover = 0;
        if (!vorbisDecode(m_readData, rlen, &leftover, false) ||
            !m_decSamplesLen) {
            DEBUG_INFO("vorbisDecode failed");
            return false;
        }
        m_readPos += rlen - leftover;

        if (*len + m_decSamplesLen > allocLen ) {
	        // Should not happen
            allocLen = m_decSamplesLen + *len;
            outBuf = (unsigned char*)realloc(outBuf, allocLen);
        	if (!outBuf) {
                DEBUG_INFO("Failed to allocate" << allocLen << "bytes!");
                return NULL;
            }
    	}
        memcpy(&outBuf[*len], m_decSamples, m_decSamplesLen);
        *len += m_decSamplesLen;
	}

	return outBuf;
}

/*!
  Return sample from given position. pos is in units of PCM samples.
*/
short VorbisDecoder::at(quint64 pos)
{
    if (!m_decSamplesLen || pos < m_decodedDataStart ||
        pos > m_decodedDataEnd) {
        if (!vorbisSeek(pos))
            return 0;
    }

    return m_decSamples[pos - m_decodedDataStart];
}

/*!
  Initializes stb vorbis.
*/
int VorbisDecoder::vorbisInit()
{
    int used = 0, error = 0;
    int len;

    unsigned char *data = readHeaderPages(&len);
    if (!data)
        return -1;

    m_vorbis = stb_vorbis_open_pushdata((unsigned char*)data, len, &used,
        &error, NULL);

    DEBUG_INFO("stb vorbis init complete, used" << used
        << "bytes, error" << error);

    if (!m_vorbis)
        return -error;

    memset(&m_info, 0, sizeof(stb_vorbis_info));
    m_info = stb_vorbis_get_info(m_vorbis);

    DEBUG_INFO("   sample_rate:" << m_info.sample_rate);
    DEBUG_INFO("      channels:" << m_info.channels);
    DEBUG_INFO("max_frame_size:" << m_info.max_frame_size);

	free(data);
	const OggPage *page = firstAudioPage();
	m_readPos = page->m_pageStartPos;
    m_readData = (unsigned char *)malloc(maxblocksize);

    return used;
}

/*!
  Frees up resources related to decoding.
*/
void VorbisDecoder::vorbisUninit()
{
    if (m_vorbis) {
        stb_vorbis_close(m_vorbis);
        m_vorbis = NULL;
    }

    free(m_readData);
    m_readData = NULL;
    free(m_decSamples);
    m_decSamples = NULL;
    m_decSamplesLen = 0;
}

/*!
  Decode a block of data. data/len defines the input data,
  m_decSamples/m_decSamplesLen contains the decoded data. leftover is the length
  of unused input data. If singleFrame is set to true this will decode only the
  first complete frame from the input data, otherwise this will try to decode
  all the complete frames.
*/
bool VorbisDecoder::vorbisDecode(unsigned char *data, int len, int *leftover,
                                 bool singleFrame)
{
    float **outputs;
    int channels, samplecount, t, i;
    int outPos = 0;
    unsigned char *p = data;

    m_decSamplesLen = 0;

	if (!m_decSamples) {
        m_maxDecSamplesLen = maxblocksize;
		m_decSamples = (short*)malloc(sizeof(short) * m_maxDecSamplesLen);
        if (!m_decSamples) {
            DEBUG_INFO("Failed to allocate" <<
                       sizeof(short) * m_maxDecSamplesLen << "bytes");
            return false;
        }
	}

    while (len) {
        int used = stb_vorbis_decode_frame_pushdata(m_vorbis, (unsigned char*)p,
            len, &channels, &outputs, &samplecount);
        if (!used && !samplecount)
            break;
        len -= used;
        p += used;
        if (outPos + samplecount * channels > m_maxDecSamplesLen) {
            m_maxDecSamplesLen += maxblocksize;
            m_decSamples = (short*)realloc(m_decSamples,
                m_maxDecSamplesLen * sizeof(short));
            if (!m_decSamples) {
                DEBUG_INFO("Failed to allocate" <<
                           sizeof(short) * m_maxDecSamplesLen << "bytes");
                return false;
            }
        }

        for (t = 0; t < samplecount; t++) {
            for (i = 0; i < channels; i++) {
                // TODO: Values are not clamped to -1...1 by stb vorbis
                m_decSamples[outPos++] = (short)(outputs[i][t] * 30000.0f);
            }
        }
        if (singleFrame)
            break;
    }
    *leftover = len;
    m_decSamplesLen = outPos * sizeof(short);
    return true;
}

/*!
  Informs stb vorbis that the the next datablock will not be contiguous with
  previous ones. This is needed when seeking. Decoding will continue from the
  beginning of next valid ogg page.
*/
bool VorbisDecoder::vorbisFlush()
{
	if (!m_vorbis)
		return false;
    stb_vorbis_flush_pushdata(m_vorbis);
    return true;
}

/*!
  Decode given page. Note that the given page defines the end of decoding,
  starting page is some of the previous pages.
*/
bool VorbisDecoder::vorbisDecodePage(const OggPage *page)
{
    const OggPage *startPage = page->m_prev;
    const OggPage *endPage = page;
    const OggPage *p = startPage;
    do {
        if (p->m_granulePosition != startPage->m_granulePosition &&
            p->m_granulePosition != (quint64)-1) {
            startPage = p;
            break;
        }
    } while((p = p->m_prev));

    m_readPos = startPage->m_pageStartPos;
    int lenToRead = endPage->m_pageStartPos - startPage->m_pageStartPos +
        endPage->m_pageLength;

    int rlen = lenToRead;
    int leftover = 0;
    if (m_cached) {
        if (!vorbisDecode(&m_cache[m_readPos], rlen, &leftover, false) ||
                !m_decSamplesLen) {
            DEBUG_INFO("vorbisDecode failed");
            return false;
        }
    } else {
        rlen = read(m_readData, lenToRead, m_readPos);
        if (!rlen)
            return false;

        if (!vorbisDecode(m_readData, rlen, &leftover, false) ||
                !m_decSamplesLen) {
            DEBUG_INFO("vorbisDecode failed");
            return false;
        }
    }
    m_readPos += rlen - leftover;

    unsigned int decPos = m_decSamplesLen / m_info.channels / sizeof(short);
    if (endPage->m_granulePosition < decPos) {
        m_decodedDataStart = 0;
    } else {
        m_decodedDataStart = (endPage->m_granulePosition - decPos -
            m_granuleOffset) * m_info.channels;
    }
    m_decodedDataEnd = (endPage->m_granulePosition - m_granuleOffset) *
            m_info.channels;
    return true;
}

/*!
  Decode next frame. This can be used only when decoding contiguously forward.
*/
bool VorbisDecoder::vorbisDecodeNext()
{
    int rlen;
    int leftover = 0;

    if (m_cached) {
        rlen = qMin(65536, m_length - m_readPos);
        if (!vorbisDecode(&m_cache[m_readPos], rlen, &leftover, true) ||
                !m_decSamplesLen) {
            DEBUG_INFO("vorbisDecode failed");
            return false;
        }
    } else {
        rlen = read(m_readData, rlen, m_readPos);
        if (!rlen)
            return false;

        if (!vorbisDecode(m_readData, rlen, &leftover, true) ||
                !m_decSamplesLen) {
            DEBUG_INFO("vorbisDecode failed");
            return false;
        }
    }
    m_readPos += rlen - leftover;

    unsigned int decLen = m_decSamplesLen / sizeof(short);
    m_decodedDataStart = m_decodedDataEnd + 1;
    m_decodedDataEnd = m_decodedDataStart + decLen - 1;
    return true;
}

/*!
  Seek relatively from current sample position. offset is in units of PCM
  samples.
*/
bool VorbisDecoder::vorbisSeekRelative(qint64 offset)
{
    if (!m_vorbis)
        return false;

    int current = stb_vorbis_get_sample_offset(m_vorbis);
    if (current == -1) {
        DEBUG_INFO("no offset yet");
        return false;
    }

    if (m_decSamplesLen && current + offset - m_decodedDataEnd <
            m_info.max_frame_size) {
        if (!vorbisDecodeNext())
            return false;
    } else {
        const OggPage *page = seekPage((current + offset) / m_info.channels);
        if (!page)
            return false;

        if (!vorbisFlush())
            return false;

        if (!vorbisDecodePage(page))
            return false;
    }

    return true;
}

/*!
  Seek to given sample position. pos is in units of PCM samples.
*/
bool VorbisDecoder::vorbisSeek(qint64 pos)
{
    if (m_decSamplesLen &&
        pos - m_decodedDataEnd < m_info.max_frame_size) {
        if (!vorbisDecodeNext())
            return false;
    } else {
        const OggPage *page = seekPage(pos / m_info.channels);
        if (!page)
            return false;

        if (!vorbisFlush())
            return false;

        if (!vorbisDecodePage(page))
            return false;
    }

    return true;
}
