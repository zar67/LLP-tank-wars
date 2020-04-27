//
// Created by z2-rowbotham on 11/03/2020.
//

#ifndef MYNETGAME_SCENEMANAGER_H
#define MYNETGAME_SCENEMANAGER_H

#include "../Audio/AudioManager.h"
#include "../InputManager.h"
#include "../Map/TileData.h"
#include "../Troop.h"
#include "scenes/GameOverScreen.h"
#include "scenes/GameScreen.h"
#include "scenes/JoinScreen.h"
#include "scenes/Lobby.h"
#include "scenes/MainMenu.h"
#include "ui_elements/UIElement.h"

#include <Engine/Renderer.h>

class InputManager;

class SceneManager
{
 public:
  enum class Screens
  {
    MAIN_MENU   = 0,
    JOIN_SCREEN = 1,
    LOBBY       = 2,
    GAME        = 3,
    GAME_OVER   = 4
  };

 public:
  SceneManager()  = default;
  ~SceneManager() = default;

  bool init(ASGE::Renderer* renderer, int font_index);

  UIElement::MenuItem update(InputManager* input_manager, std::array<int, 2> cam_pos);
  void render(ASGE::Renderer* renderer);
  void renderGameScreen(
    ASGE::Renderer* renderer,
    int action_number,
    int current_player_turn,
    bool in_turn,
    bool alive,
    Troop* troop_selected,
    const std::vector<std::vector<Troop*>>& troops,
    Map* map,
    int currency);

  void screenOpen(Screens screen);
  Screens screenOpen() { return screen_open; };

  JoinScreen* joinScreen();
  Lobby* lobbyScreen();
  GameScreen* gameScreen();
  GameOverScreen* gameOverScreen();

 private:
  MainMenu main_menu       = MainMenu();
  JoinScreen join_screen   = JoinScreen();
  Lobby lobby              = Lobby();
  GameScreen game_screen   = GameScreen();
  GameOverScreen game_over = GameOverScreen();

  Screens screen_open = Screens::MAIN_MENU;

  AudioManager audio;
};

#endif  // MYNETGAME_SCENEMANAGER_H
