//
// Created by z2-rowbotham on 13/03/2020.
//

#ifndef MYNETGAME_JOINSCREEN_H
#define MYNETGAME_JOINSCREEN_H

#include "../ui_elements/Button.h"
#include "../ui_elements/TextBox.h"
#include "../ui_elements/UIElement.h"
#include <Engine/Renderer.h>

class JoinScreen
{
 public:
  JoinScreen()  = default;
  ~JoinScreen() = default;

  bool init(ASGE::Renderer* renderer, int font_index, int game_width);
  UIElement::MenuItem
  update(const ASGE::Point2D& cursor_pos, bool click, bool key_pressed, int key);
  void render(ASGE::Renderer* renderer);

  std::string getIP();

 private:
  ASGE::Text title;
  TextBox text_box;
  Button start_game;
};

#endif // MYNETGAME_JOINSCREEN_H
