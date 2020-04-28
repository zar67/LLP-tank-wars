//
// Created by Ryan on 27/03/2020.
//

#include "Audio/AudioManager.h"

#include <Audio/AudioManager.h>

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

  return !(
    !loadFile("/data/Audio/Background-ingame.mp3", Background_MP3) ||
    !loadFile("/data/Audio/click_004.mp3", click_004_MP3) ||
    !loadFile("/data/Audio/Buy_Unit.mp3", Buy_Unit_MP3) ||
    !loadFile("/data/Audio/Deselection.mp3", Deselection_MP3) ||
    !loadFile("/data/Audio/Game_Menu.mp3", Game_Menu_MP3) ||
    !loadFile("/data/Audio/Movement.mp3", Movement_MP3) ||
    !loadFile("/data/Audio/Player_Joined.mp3", Player_Joined_MP3) ||
    !loadFile("/data/Audio/Selection.mp3", Selection_MP3) ||
    !loadFile("/data/Audio/Shoot.mp3", Shoot_MP3) ||
    !loadFile("/data/Audio/Explosion.mp3", Explosion_MP3));
}

void AudioManager::playClick()
{
  soloud.play(click_004_MP3);
}

void AudioManager::playBackgroundMusic()
{
  soloud.stopAll();
  Background_MP3.setLooping(true);
  soloud.play(Background_MP3);
}

void AudioManager::playDeselection()
{
  soloud.play(Deselection_MP3);
}

void AudioManager::playbuyUnit()
{
  soloud.play(Buy_Unit_MP3);
}

void AudioManager::playplayerJoined()
{
  soloud.play(Player_Joined_MP3);
}

void AudioManager::playgameMenu()
{
  Game_Menu_MP3.setLooping(true);
  soloud.play(Game_Menu_MP3);
}

void AudioManager::playSelection()
{
  soloud.play(Selection_MP3);
}

void AudioManager::playMovement()
{
  soloud.play(Movement_MP3);
}

void AudioManager::playExplosion()
{
  soloud.play(Explosion_MP3);
}

void AudioManager::playShoot()
{
  soloud.play(Shoot_MP3);
}

void AudioManager::stopAudio()
{
  soloud.stopAll();
}
