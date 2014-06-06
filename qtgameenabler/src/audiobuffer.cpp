/**
 * Copyright (c) 2011-2012 Nokia Corporation.
 * All rights reserved.
 *
 * Part of the Qt GameEnabler.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */

#include "audiobuffer.h"

#include <QFile>
#include <stdlib.h>

#include "audiobufferplayinstance.h"
#include "audiomixer.h"
#include "trace.h"
#ifdef QTGAMEENABLER_SUPPORT_OGG_VORBIS
#include "vorbisdecoder.h"
#endif // QTGAMEENABLER_SUPPORT_OGG_VORBIS

using namespace GE;


/*!
  Header for wav data
*/
struct SWavHeader {
    char chunkID[4];
    unsigned int chunkSize;
    char format[4];

    unsigned char subchunk1id[4];
    unsigned int subchunk1size;
    unsigned short audioFormat;
    unsigned short nofChannels;
    unsigned int sampleRate;
    unsigned int byteRate;

    unsigned short blockAlign;
    unsigned short bitsPerSample;

    unsigned char subchunk2id[4];
    unsigned int subchunk2size;
};


/*!
 * \class AudioBuffer
 * \brief A class to hold audio information (a buffer).
 */


/*!
  Constructor.
*/
AudioBuffer::AudioBuffer(QObject *parent)
    : QObject(parent),
      m_sampleFunction(NULL),
      m_data(NULL),
      m_dataLength(0),
      m_nofChannels(0),
      m_bitsPerSample(0),
      m_signedData(false),
      m_samplesPerSec(0)
{
    DEBUG_INFO(this);
}


/*!
  Destructor.
*/
AudioBuffer::~AudioBuffer()
{
    // Deallocate the data.
    reallocate(0);
}


/*!
  (Re)allocates the audio buffer according to \a length.
*/
void AudioBuffer::reallocate(int length)
{
    if (m_data)
        delete [] ((char*)m_data);

    m_dataLength = length;

    if (m_dataLength > 0)
        m_data = new char[m_dataLength];
    else
        m_data = 0;
}

/*!
  Loads a .wav or .ogg file. Can be used for loading from Qt resources as well.
  If \a parent is given, it is set as the parent of the constructed buffer.

  Returns a new buffer if successful, NULL otherwise.
*/
AudioBuffer *AudioBuffer::load(QString fileName, QObject *parent)
{
    AudioBuffer *buffer;

    buffer = AudioBuffer::loadWav(fileName, parent);
    if (!buffer) {
        DEBUG_INFO("Trying to load the file as an ogg file");
        buffer = AudioBuffer::loadOgg(fileName, parent);
    }
    return buffer;
}

/*!
  Loads a .wav file from file with \a fileName. Note that this method can be
  used for loading .wav from Qt resources as well. If \a parent is given, it
  is set as the parent of the constructed buffer.

  Returns a new buffer if successful, NULL otherwise.
*/
AudioBuffer *AudioBuffer::loadWav(QString fileName, QObject *parent)
{
    QFile wavFile(fileName);

    if (wavFile.open(QIODevice::ReadOnly)) {
        AudioBuffer *buffer = loadWav(wavFile, parent);

        if (!buffer)
            DEBUG_INFO("Failed to load data from " << fileName << "!");

        wavFile.close();
        return buffer;
    }

    DEBUG_INFO("Failed to open " << fileName << ": " << wavFile.errorString());
    return NULL;
}

/*!
  Loads a .ogg file from file with \a fileName. Note that this method can be
  used for loading .ogg from Qt resources as well. If \a parent is given, it
  is set as the parent of the constructed buffer.

  Returns a new buffer if successful, NULL otherwise.
*/
AudioBuffer *AudioBuffer::loadOgg(QString fileName, QObject *parent)
{
    AudioBuffer *buffer = new AudioBuffer(parent);

    if (!buffer->readOgg(fileName)) {
        DEBUG_INFO("Failed to read ogg file:" << fileName);
        delete buffer;
        return NULL;
    }
    return buffer;
}

/*!
  Protected method, called from AudioBuffer::loadWav(QString, QObject*).

  Loads a .wav file from a preopened \a wavFile. If \a parent is given, it is
  set as the parent of the constructed buffer.

  Returns a new buffer if successful, NULL otherwise.
*/
AudioBuffer *AudioBuffer::loadWav(QFile &wavFile, QObject *parent)
{
    if (!wavFile.isOpen()) {
        // The file is not open!
        DEBUG_INFO("The given file must be opened before calling this method!");
        return NULL;
    }

    SWavHeader header;

    wavFile.read(header.chunkID, 4);

    if (header.chunkID[0] != 'R' || header.chunkID[1] != 'I' ||
        header.chunkID[2] != 'F' || header.chunkID[3] != 'F') {
        // Incorrect header
        return NULL;
    }

    wavFile.read((char*)&header.chunkSize, 4);
    wavFile.read((char*)&header.format, 4);

    if (header.format[0] != 'W' || header.format[1] != 'A' ||
        header.format[2] != 'V' || header.format[3] != 'E') {
        // Incorrect header
        return NULL;
    }

    wavFile.read((char*)&header.subchunk1id, 4);

    if (header.subchunk1id[0] != 'f' || header.subchunk1id[1] != 'm' ||
        header.subchunk1id[2] != 't' || header.subchunk1id[3] != ' ') {
        // Incorrect header
        return NULL;
    }

    wavFile.read((char*)&header.subchunk1size, 4);
    wavFile.read((char*)&header.audioFormat, 2);
    wavFile.read((char*)&header.nofChannels, 2);
    wavFile.read((char*)&header.sampleRate, 4);
    wavFile.read((char*)&header.byteRate, 4);
    wavFile.read((char*)&header.blockAlign, 2);
    wavFile.read((char*)&header.bitsPerSample, 2);

    while (1) {
        if (wavFile.read((char*)&header.subchunk2id, 4) != 4)
            return NULL;

        if (wavFile.read((char*)&header.subchunk2size, 4) != 4)
            return NULL;

        if (header.subchunk2id[0] == 'd' && header.subchunk2id[1] == 'a' &&
            header.subchunk2id[2] == 't' && header.subchunk2id[3] == 'a') {
            // Found the data chunk.
            break;
        }

        // This was not the data-chunk. Skip it.
        if (header.subchunk2size < 1) {
            // Error in file!
            return NULL;
        }

        char *unused = new char[header.subchunk2size];
        wavFile.read(unused, header.subchunk2size);
        delete [] unused;
    }

    // The data follows.
    if (header.subchunk2size < 1)
        return NULL;

    // Construct the buffer.
    AudioBuffer *buffer = new AudioBuffer(parent);

    buffer->m_nofChannels = header.nofChannels;
    buffer->m_bitsPerSample = header.bitsPerSample;
    buffer->m_samplesPerSec = header.sampleRate;
    buffer->m_signedData = 0; // Where to look for this?
    buffer->reallocate(header.subchunk2size);

    wavFile.read((char*)buffer->m_data, header.subchunk2size);

    // Select a good sampling function.
    if (!setSampleFunction(*buffer)) {
        // Failed to resolve the sample function!
        delete buffer;
        return NULL;
    }

    return buffer;
}

/*!
  Protected method, called from AudioBuffer::loadOgg(QString, QObject*).

  Decodes an entire .ogg file into memory.

  Returns true if if successful.
*/
bool AudioBuffer::readOgg(QString &oggFile)
{
#ifdef QTGAMEENABLER_SUPPORT_OGG_VORBIS
    VorbisDecoder decoder(false, this);

    if (!decoder.load(oggFile))
        return false;
    
    const stb_vorbis_info *info = decoder.fileInfo();
    m_nofChannels = info->channels;
    m_bitsPerSample = 16;
    m_samplesPerSec = info->sample_rate;
    m_signedData = 0;
    setSampleFunction(*this);

    unsigned int len;
    m_data = decoder.decodeAll(&len);
    if (!m_data)
        return false;

    m_dataLength = len;
    return true;
#else
    return false;
#endif
}

// Mix to  mono versions.

AUDIO_SAMPLE_TYPE AudioBuffer::sampleFunction8bitMono(AudioBuffer *buffer,
                                                      int pos,
                                                      int channel)
{
    Q_UNUSED(channel);
    return (AUDIO_SAMPLE_TYPE)(((quint8*)(buffer->m_data))[pos] - 128) << 8;
}


AUDIO_SAMPLE_TYPE AudioBuffer::sampleFunction16bitMono(AudioBuffer *buffer,
                                                        int pos,
                                                        int channel)
{
    Q_UNUSED(channel);
    return (AUDIO_SAMPLE_TYPE)(((quint16*)(buffer->m_data))[pos]);
}


AUDIO_SAMPLE_TYPE AudioBuffer::sampleFunction32bitMono(AudioBuffer *buffer,
                                                        int pos,
                                                        int channel)
{
    Q_UNUSED(channel); // To prevent compiler warnings.
    return (((float*)(buffer->m_data))[pos * buffer->m_nofChannels]) * 65536.0f / 2.0f;
}


// Mix to stereo versions.

AUDIO_SAMPLE_TYPE AudioBuffer::sampleFunction8bitStereo(AudioBuffer *buffer,
                                                         int pos,
                                                         int channel)
{
    return ((AUDIO_SAMPLE_TYPE)
        (((quint8*)(buffer->m_data))[pos * buffer->m_nofChannels + channel]) << 8);
}


AUDIO_SAMPLE_TYPE AudioBuffer::sampleFunction16bitStereo(AudioBuffer *buffer,
                                                          int pos,
                                                          int channel)
{
    return (AUDIO_SAMPLE_TYPE)
        (((quint16*)(buffer->m_data))[pos * buffer->m_nofChannels + channel]);
}


AUDIO_SAMPLE_TYPE AudioBuffer::sampleFunction32bitStereo(AudioBuffer *buffer,
                                                          int pos,
                                                          int channel)
{
    return (((float*)(buffer->m_data))[pos * buffer->m_nofChannels +
            channel]) * 65536.0f / 2.0f;
}


/*!
  Constructs a new play instance and sets it as an audio source for \a mixer.
  Note that the mixer takes ownership of the constructed instance.
  Returns the constructed instance or NULL in case the mixer refused to add
  the instance.
*/
AudioBufferPlayInstance *AudioBuffer::playWithMixer(AudioMixer &mixer)
{
    AudioBufferPlayInstance *instance = new AudioBufferPlayInstance(this);

    if (!mixer.addAudioSource(instance)) {
        DEBUG_INFO("Failed to add the new audio source to mixer!");
        delete instance;
        return NULL;
    }

    return instance;
}


/*!
  Sets an appropriate sample function for \a buffer depending on the number of
  channels and the bit rate.

  Returns true if successful, false otherwise.
*/
bool AudioBuffer::setSampleFunction(AudioBuffer &buffer)
{
    buffer.m_sampleFunction = 0;

    if (buffer.m_nofChannels == 1) {
        if (buffer.m_bitsPerSample == 8)
            buffer.m_sampleFunction = sampleFunction8bitMono;

        if (buffer.m_bitsPerSample == 16)
            buffer.m_sampleFunction = sampleFunction16bitMono;

        if (buffer.m_bitsPerSample == 32)
            buffer.m_sampleFunction = sampleFunction32bitMono;
    } else {
        if (buffer.m_bitsPerSample == 8)
            buffer.m_sampleFunction = sampleFunction8bitStereo;

        if (buffer.m_bitsPerSample == 16)
            buffer.m_sampleFunction = sampleFunction16bitStereo;

        if (buffer.m_bitsPerSample == 32)
            buffer.m_sampleFunction = sampleFunction32bitStereo;
    }

    if (!buffer.m_sampleFunction) {
        // Unknown bit rate!
        DEBUG_INFO("Unknown bit rate:" << buffer.m_bitsPerSample);
        return false;
    }

    return true;
}
