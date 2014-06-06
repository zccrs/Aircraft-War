/**
 * Copyright (c) 2011-2012 Nokia Corporation.
 * All rights reserved.
 *
 * Part of the Qt GameEnabler.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */

#ifndef GE_VORBISDECODER_H
#define GE_VORBISDECODER_H

#include <QFile>
#include "stb_vorbis.h"

namespace GE {

// Forward declarations (inside GE namespace)
class OggPage;

// Ogg vorbis decoder using stb vorbis in push mode
class VorbisDecoder : public QObject
{
    Q_OBJECT

public:
    explicit VorbisDecoder(bool cached = false, QObject *parent = NULL);
    virtual ~VorbisDecoder();

public:
    bool load(QString &filename);
    inline quint64 decodedLength() { return m_decodedLength; }
    unsigned char *decodeAll(unsigned int *len);
    const stb_vorbis_info *fileInfo();
    short at(quint64 pos);

private:
    bool scan();
    OggPage *readPageHeader();
    const OggPage *seekPage(quint64 samplePos);
    const OggPage *firstAudioPage();
    unsigned char *readHeaderPages(int *len);
    bool vorbisSeek(qint64 pos);
    bool vorbisSeekRelative(qint64 offset);
    bool vorbisDecodePage(const OggPage *page);
    bool vorbisDecodeNext();
    bool vorbisFlush();
    bool vorbisDecode(unsigned char *data, int len,
                      int *leftover, bool singleFrame);
    void vorbisUninit();
    int vorbisInit();
    int read(unsigned char *buf, int len, int pos);
    bool seek(int pos);
    int tell();

private:
    QFile m_file;
    OggPage *m_pageList;
    int m_pos;
    int m_length;
    bool m_cached;
    unsigned char *m_cache;
    quint64 m_decodedDataStart;
    quint64 m_decodedDataEnd;
    stb_vorbis_info m_info;
    stb_vorbis *m_vorbis;
    short *m_decSamples;
    int m_decSamplesLen;
	int m_maxDecSamplesLen;
    int m_readPos;
    unsigned char *m_readData;
    quint64 m_decodedLength;
    qint64 m_granuleOffset;
};

} // namespace GE

#endif // GE_VORBISDECODER_H
