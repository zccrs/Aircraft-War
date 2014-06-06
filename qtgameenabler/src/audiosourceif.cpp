/**
 * Copyright (c) 2011-2012 Nokia Corporation.
 * All rights reserved.
 *
 * Part of the Qt GameEnabler.
 *
 * For the applicable distribution terms see the license text file included in
 * the distribution.
 */

#include "audiosourceif.h"

using namespace GE;


/*!
  \class AudioSource
  \brief An abstract interface for an audio source.
*/


/*!
  Constructor.
*/
AudioSource::AudioSource(QObject *parent /* = 0 */)
    : QObject(parent)
{
}


/*!
  Destructor.
*/
AudioSource::~AudioSource()
{
}


/*!
  This method is used to check whether this instance can be (auto-)destroyed
  or not.

  To be implemented in the derived class. This default implementation always
  returns false.
*/
bool AudioSource::canBeDestroyed()
{
    return false;
}
