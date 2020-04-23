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

bool AudioManager::loadFile(const std::string& filename, SoLoud::Wav& dest)
{
  ASGE::FILEIO::File mp3;
  if (!mp3.open(filename))
  {
    return false;
  }
  auto io_buffer = mp3.read();
  if (
    dest.loadMem(
      io_buffer.as_unsigned_char(), static_cast<unsigned int>(io_buffer.length), false, false) ==
    SoLoud::FILE_LOAD_FAILED)
  {
    return false;
  }

  dest.setVolume(0.5);
  mp3.close();

  return true;
}

bool AudioManager::audioSetUp()
{
  /// thread audio
  // std::thread audio_thread;
  // audio_thread.detach();

  if (soloud.init() != SoLoud::SO_NO_ERROR)
  {
    return false;
  }

  if (!loadFile("/data/Audio/Background.mp3", Background_MP3))
  {
    return false;
  }

  return true;
}

void AudioManager::playClick()
{
  soloud.play(click_004_MP3);
}

void AudioManager::playBackgroundMusic()
{
  soloud.play(Background_MP3);
}