//
// Created by z2-rowbotham on 11/03/2020.
//

#ifndef MYNETGAME_GAMESCREEN_H
#define MYNETGAME_GAMESCREEN_H

#include "../../Troop.h"
#include "../ui_elements/Button.h"
#include "../ui_elements/UIElement.h"
#include "Shop.h"

#include <Engine/Renderer.h>
#include <vector>
#include "../../Audio/AudioManager.h"

class GameScreen
{
 public:
  GameScreen() = default;
  ~GameScreen();

  GameScreen(const GameScreen& gameScreen){};           // Copy Constructor
  GameScreen& operator=(const GameScreen& gameScreen);  // Copy Assignment Operator

  bool init(ASGE::Renderer* renderer, int font_index);
  UIElement::MenuItem update(AudioManager* audio_manager, const ASGE::Point2D& cursor_pos, std::atomic<bool>& click);
  void render(
    ASGE::Renderer* renderer,
    int action_number,
    int current_player_turn,
    bool in_turn,
    Troop* troop_selected,
    const int& currency);

  bool initShop(ASGE::Renderer* renderer, int font_index, int player_id);
  void openShop();
  void closeShop();

 private:
  Shop shop;
  Button open_shop;
  Button end_turn;
  ASGE::Sprite* selected_box = nullptr;

  bool shop_active = false;
};

#endif  // MYNETGAME_GAMESCREEN_H
