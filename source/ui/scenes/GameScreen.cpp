//
// Created by z2-rowbotham on 11/03/2020.
//

#include "ui/scenes/GameScreen.h"

bool GameScreen::init(
  ASGE::Renderer* renderer,
  int font_index,
  const std::vector<std::string>& unit_types)
{
  if (!shop.init(renderer, font_index, unit_types))
  {
    return false;
  }

  if (!open_shop.init(
        renderer,
        font_index,
        "data/sprites/ui/button.png",
        "data/sprites/ui/button_pressed.png",
        "Open Shop",
        10,
        10,
        290,
        30))
  {
    return false;
  }

  return end_turn.init(
    renderer,
    font_index,
    "data/sprites/ui/button.png",
    "data/sprites/ui/button_pressed.png",
    "End Turn",
    static_cast<float>(ASGE::SETTINGS.window_width) - 260,
    static_cast<float>(ASGE::SETTINGS.window_height) - 40,
    250,
    30);
}

UIElement::MenuItem GameScreen::update(const ASGE::Point2D& cursor_pos, bool click)
{
  open_shop.update(cursor_pos, click);

  if (open_shop.pressed())
  {
    if (shop_active)
    {
      closeShop();
    }
    else
    {
      openShop();
    }
    return UIElement::MenuItem::NONE;
  }

  UIElement::MenuItem item = shop.update(cursor_pos, click);
  end_turn.update(cursor_pos, click);

  if (item == UIElement::MenuItem::MAP_CLICK && !shop_active)
  {
    item = UIElement::MenuItem ::NONE;
  }
  if (end_turn.pressed())
  {
    item = UIElement::MenuItem::END_TURN;
  }

  return item;
}

void GameScreen::render(
  ASGE::Renderer* renderer,
  int current_player_turn,
  bool in_turn,
  const int& currency)
{
  renderer->renderText("GAME", 300, 300, ASGE::COLOURS::WHITE);
  renderer->renderText("Currency: " + std::to_string(currency), 310, 35, ASGE::COLOURS::WHITE);

  open_shop.render(renderer);

  if (shop_active)
  {
    shop.render(renderer);
  }

  if (in_turn)
  {
    renderer->renderText("YOUR TURN", 15, ASGE::SETTINGS.window_height - 15, ASGE::COLOURS::WHITE);
    end_turn.render(renderer);
  }
  else
  {
    renderer->renderText(
      "PLAYER " + std::to_string(current_player_turn) + "'S TURN",
      15,
      ASGE::SETTINGS.window_height - 15,
      ASGE::COLOURS::WHITE);
  }
}

void GameScreen::openShop()
{
  shop_active = true;
  open_shop.changeText("Close Shop");
}

void GameScreen::closeShop()
{
  shop_active = false;
  open_shop.changeText("Open Shop");
}