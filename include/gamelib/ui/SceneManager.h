//
// Created by z2-rowbotham on 11/03/2020.
//

#ifndef MYNETGAME_SCENEMANAGER_H
#define MYNETGAME_SCENEMANAGER_H

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
  update(const ASGE::Point2D& cursor_pos, bool click, bool key_pressed, int key);
  void render(ASGE::Renderer* renderer);

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
  const int currency  = 100;  // TEMP VARIABLE TO TEST UI, SHOULD PLAYER_MOVE TO PLAYER CLASS
                              // WHEN MADE
};

#endif  // MYNETGAME_SCENEMANAGER_H
