//
// Created by Ryan on 27/03/2020.
//

#ifndef MYNETGAME_AUDIOMANAGER_H
#define MYNETGAME_AUDIOMANAGER_H

#include <Engine/FileIO.h>
#include <Engine/Logger.hpp>
#include <Engine/OGLGame.h>
#include <soloud.h>
#include <soloud_wav.h>
#include <thread>

class AudioManager
{
 public:
  AudioManager() = default;
  ~AudioManager();

  AudioManager(const AudioManager& audio){};
  AudioManager& operator=(AudioManager audio);

  bool audioSetUp();
  bool loadFile(const std::string& filename, SoLoud::Wav& dest);

  void playClick();
  void playBackgroundMusic();
  void playbuyUnit();
  void playDeselection();
  void playgameMenu();
  void playMovement();
  void playplayerJoined();
  void playSelection();
  void playShoot();
  void playExplosion();

  void stopAudio();

 private:
  SoLoud::Wav click_004_MP3;
  SoLoud::Wav Background_MP3;
  SoLoud::Wav Buy_Unit_MP3;
  SoLoud::Wav Deselection_MP3;
  SoLoud::Wav Game_Menu_MP3;
  SoLoud::Wav Movement_MP3;
  SoLoud::Wav Player_Joined_MP3;
  SoLoud::Wav Selection_MP3;
  SoLoud::Wav Shoot_MP3;
  SoLoud::Wav Explosion_MP3;

  SoLoud::Soloud soloud;
};
#endif  // MYNETGAME_AUDIOMANAGER_H