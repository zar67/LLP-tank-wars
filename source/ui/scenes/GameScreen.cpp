//
// Created by z2-rowbotham on 11/03/2020.
//

#include "ui/scenes/GameScreen.h"

#include <Engine/Logger.hpp>

bool GameScreen::init(ASGE::Renderer* renderer, int font_index)
{
  if (!open_shop.init(
        renderer,
        font_index,
        "data/sprites/ui/button.png",
        "data/sprites/ui/button_pressed.png",
        "Open Shop",
        10,
        10,
        260,
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

UIElement::MenuItem GameScreen::update(
  const ASGE::Point2D& cursor_pos,
  std::atomic<bool>& click,
  std::array<int, 2> camera_pos)
{
  open_shop.update(cursor_pos, click, camera_pos);

  if (open_shop.pressed())
  {
    click = false;
    if (shop_active)
    {
      closeShop();
    }
    else
    {
      openShop();
    }
    return UIElement::MenuItem::SHOP_BUTTON;
  }

  end_turn.update(cursor_pos, click, camera_pos);
  if (end_turn.pressed())
  {
    click = false;
    return UIElement::MenuItem::END_TURN;
  }

  UIElement::MenuItem item = UIElement::MenuItem::NONE;

  if (shop_active)
  {
    item = shop.update(cursor_pos, click, camera_pos);
  }

  if (click)
  {
    item = UIElement::MenuItem::MAP_CLICK;
  }

  return item;
}

void GameScreen::render(
  ASGE::Renderer* renderer,
  int action_number,
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
    renderer->renderText(
      "ACTIONS: " + std::to_string(action_number) + "/3",
      15,
      ASGE::SETTINGS.window_height - 45,
      ASGE::COLOURS::WHITE);
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

bool GameScreen::initShop(ASGE::Renderer* renderer, int font_index, int player_id)
{
  return shop.init(renderer, font_index, player_id);
}

void GameScreen::openShop()
{
  shop_active = true;
  shop_title  = "Close Shop";
  open_shop.changeText("Close Shop");
}

void GameScreen::closeShop()
{
  shop_active = false;
  shop_title  = "Open Shop";
  open_shop.changeText("Open Shop");
}