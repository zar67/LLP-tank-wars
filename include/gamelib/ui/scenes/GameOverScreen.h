//
// Created by zar67 on 15/04/2020.
//

#ifndef MYNETGAME_GAMEOVERSCREEN_H
#define MYNETGAME_GAMEOVERSCREEN_H

#include "../ui_elements/Button.h"
#include "../ui_elements/UIElement.h"

#include <Engine/Renderer.h>

class GameOverScreen
{
 public:
  GameOverScreen()  = default;
  ~GameOverScreen() = default;

  bool init(ASGE::Renderer* renderer, int font_index);
  UIElement::MenuItem update(const ASGE::Point2D& cursor_pos, std::atomic<bool>& click);
  void render(ASGE::Renderer* renderer);

  void setWinningValues(int player_id);

 private:
  ASGE::Text title;
  Button main_menu;
  Button exit_game;

  int winning_player = -1;
};

#endif  // MYNETGAME_GAMEOVERSCREEN_H
