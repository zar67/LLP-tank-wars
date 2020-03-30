//
// Created by Ryan on 27/03/2020.
//

#include "Audio/AudioManager.h"

AudioManager::~AudioManager()
{
  soloud.deinit();
}

AudioManager& AudioManager::operator=(const AudioManager audio)
{
  if (&audio != this)
  {
    // delete pointers
  }

  return *this;
}

bool AudioManager::audioSetUp()
{
  if (soloud.init() != SoLoud::SO_NO_ERROR)
  {
    return false;
  }

  // soloud.setVolume(1,50);

  /*if (!click_004.open("/data/Audio/click_004.mp3"))
  {
    auto io_buffer = click_004.read();
    click_004_MP3.loadMem(
      io_buffer.as_unsigned_char(), static_cast<unsigned int>(io_buffer.length), false, false);
    click_004.close();
  }
  else
  {
      return false;
  }*/

  if (!Background.open("/data/Audio/Background.mp3"))
  {
    auto io_buffer = Background.read();
    if (Background_MP3.loadMem(
          io_buffer.as_unsigned_char(), static_cast<unsigned int>(io_buffer.length), false, false))
    {
      return true;
    }
    Background.close();
  }
  else
  {
    return false;
  }
}

void AudioManager::playClick()
{
  soloud.play(click_004_MP3);
}

void AudioManager::playBackgroundMusic()
{
  soloud.play(Background_MP3);
}