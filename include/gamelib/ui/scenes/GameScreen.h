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

  bool init(ASGE::Renderer* renderer, int font_index);
  UIElement::MenuItem update(const ASGE::Point2D& cursor_pos, std::atomic<bool>& click);
  void render(
    ASGE::Renderer* renderer,
    int action_number,
    int current_player_turn,
    bool in_turn,
    const int& currency);

  bool initShop(ASGE::Renderer* renderer, int font_index, int player_id);
  void openShop();
  void closeShop();

 private:
  Shop shop;
  Button open_shop;
  Button end_turn;

  bool shop_active = false;
};

#endif  // MYNETGAME_GAMESCREEN_H
