//
// Created by z2-rowbotham on 11/03/2020.
//

#ifndef MYNETGAME_SCENEMANAGER_H
#define MYNETGAME_SCENEMANAGER_H

#include "../Audio/AudioManager.h"
#include "../InputManager.h"
#include "../Map/TileData.h"
#include "../Troop.h"
#include "../map/TileData.h"
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
    GAME        = 3
  };

 public:
  SceneManager()  = default;
  ~SceneManager() = default;

  bool init(ASGE::Renderer* renderer, int font_index, int player_id);

  UIElement::MenuItem update(InputManager* input_manager);
  void render(
    ASGE::Renderer* renderer,
    int action_number,
    int current_player_turn,
    bool in_turn,
    const std::vector<std::vector<Troop*>>& troops,
    const std::vector<TileData>& tile_data,
    int currency);
  void renderGameScreen(
    ASGE::Renderer* renderer,
    int action_number,
    int current_player_turn,
    bool in_turn,
    const std::vector<std::vector<Troop*>>& troops,
    const std::vector<TileData>& tile_data,
    int currency);

  void screenOpen(Screens screen);

  JoinScreen* joinScreen();
  Lobby* lobbyScreen();
  GameScreen* gameScreen();

 private:
  MainMenu main_menu     = MainMenu();
  JoinScreen join_screen = JoinScreen();
  Lobby lobby            = Lobby();
  GameScreen game_screen = GameScreen();

  Screens screen_open = Screens::MAIN_MENU;

  AudioManager audio;
};

#endif  // MYNETGAME_SCENEMANAGER_H
