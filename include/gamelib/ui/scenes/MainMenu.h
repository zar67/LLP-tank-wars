//
// Created by z2-rowbotham on 11/03/2020.
//

#ifndef MYNETGAME_MAINMENU_H
#define MYNETGAME_MAINMENU_H

#include "../ui_elements/Button.h"
#include "../ui_elements/UIElement.h"
#include <Engine/Renderer.h>

class MainMenu
{
 public:
  MainMenu()  = default;
  ~MainMenu() = default;

  bool init(ASGE::Renderer* renderer, int font_index);
  UIElement::MenuItem update(const ASGE::Point2D& cursor_pos, bool click);
  void render(ASGE::Renderer* renderer);

 private:
  ASGE::Text menu_title;
  Button host_game;
  Button join_game;
  Button exit_game;
};

#endif // MYNETGAME_MAINMENU_H
