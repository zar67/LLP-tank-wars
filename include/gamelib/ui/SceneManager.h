//
// Created by z2-rowbotham on 11/03/2020.
//

#ifndef MYNETGAME_SCENEMANAGER_H
#define MYNETGAME_SCENEMANAGER_H

#include "scenes/GameScreen.h"
#include "scenes/Lobby.h"
#include "scenes/MainMenu.h"
#include "ui_elements/UIElement.h"
#include <Engine/Renderer.h>

class SceneManager
{
 public:
  enum class Screens
  {
    MAIN_MENU = 0,
    LOBBY     = 1,
    GAME      = 2
  };

 public:
  SceneManager()  = default;
  ~SceneManager() = default;

  bool init(ASGE::Renderer* renderer, int font_index, int game_width);

  UIElement::MenuItem update(const ASGE::Point2D& cursor_pos, bool click);
  void render(ASGE::Renderer* renderer);

  void screenOpen(Screens screen);
  bool inMenu();

  void openShop();
  void closeShop();

 private:
  MainMenu main_menu     = MainMenu();
  Lobby lobby            = Lobby();
  GameScreen game_screen = GameScreen();

  Screens screen_open = Screens::MAIN_MENU;
  int currency = 100; // TEMP VARIABLE TO TEST UI, SHOULD MOVE TO PLAYER CLASS
                      // WHEN MADE
};

#endif // MYNETGAME_SCENEMANAGER_H
