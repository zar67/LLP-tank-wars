//
// Created by z2-rowbotham on 11/03/2020.
//

#ifndef MYNETGAME_SCENEMANAGER_H
#define MYNETGAME_SCENEMANAGER_H

#include "../Map/TileData.h"
#include "../Troop.h"
#include "../map/TileData.h"
#include "scenes/GameScreen.h"
#include "scenes/JoinScreen.h"
#include "scenes/Lobby.h"
#include "scenes/MainMenu.h"
#include "ui_elements/UIElement.h"

#include <Engine/Renderer.h>

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

  bool init(ASGE::Renderer* renderer, int font_index);

  UIElement::MenuItem
  update(const ASGE::Point2D& cursor_pos, bool click, std::atomic<bool>& key_pressed, int key);
  void render(
    ASGE::Renderer* renderer,
    int current_player_turn,
    bool in_turn,
    const std::vector<std::vector<Troop>>& troops,
    const std::vector<TileData>& tile_data,
    int currency);
  void renderGameScreen(
    ASGE::Renderer* renderer,
    int current_player_turn,
    bool in_turn,
    const std::vector<std::vector<Troop>>& troops,
    const std::vector<TileData>& tile_data,
    int currency);

  void screenOpen(Screens screen);
  bool inMenu();

  MainMenu* mainMenu();
  JoinScreen* joinScreen();
  Lobby* lobbyScreen();
  GameScreen* gameScreen();

 private:
  MainMenu main_menu     = MainMenu();
  JoinScreen join_screen = JoinScreen();
  Lobby lobby            = Lobby();
  GameScreen game_screen = GameScreen();

  Screens screen_open = Screens::MAIN_MENU;
};

#endif  // MYNETGAME_SCENEMANAGER_H
