//
// Created by z2-rowbotham on 11/03/2020.
//

#ifndef MYNETGAME_GAMESCREEN_H
#define MYNETGAME_GAMESCREEN_H

#include "../ui_elements/Button.h"
#include "../ui_elements/UIElement.h"
#include "Shop.h"
#include <Engine/Renderer.h>
#include <vector>

class GameScreen
{
 public:
  GameScreen()  = default;
  ~GameScreen() = default;

  bool init(
    ASGE::Renderer* renderer, int font_index, int game_width,
    const std::vector<std::string>& unit_types);
  UIElement::MenuItem update(const ASGE::Point2D& cursor_pos, bool click);
  void render(ASGE::Renderer* renderer, const int& currency);

  void openShop();
  void closeShop();

 private:
  Shop shop;
  Button open_shop;
};

#endif // MYNETGAME_GAMESCREEN_H
