//
// Created by Ryan on 27/03/2020.
//

#ifndef MYNETGAME_AUDIOMANAGER_H
#define MYNETGAME_AUDIOMANAGER_H

#include <Engine/FileIO.h>
#include <Engine/OGLGame.h>
#include <soloud.h>
#include <soloud_wav.h>

class AudioManager
{
 public:
  AudioManager() = default;
  ~AudioManager();

  AudioManager(const AudioManager& audio){};
  AudioManager& operator=(AudioManager audio);

  bool audioSetUp();

  void playClick();
  void playBackgroundMusic();

 private:
  ASGE::FILEIO::File click_004;
  SoLoud::Wav click_004_MP3;

  ASGE::FILEIO::File Background;
  SoLoud::Wav Background_MP3;

  SoLoud::Soloud soloud;
};
#endif  // MYNETGAME_AUDIOMANAGER_H