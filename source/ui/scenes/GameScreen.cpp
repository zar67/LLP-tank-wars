//
// Created by z2-rowbotham on 11/03/2020.
//

#include "ui/scenes/GameScreen.h"

bool GameScreen::init(
  ASGE::Renderer* renderer, int font_index, int game_width,
  const std::vector<std::string>& unit_types)
{
  if (!shop.init(renderer, font_index, game_width, unit_types))
  {
    return false;
  }

  return open_shop.init(
    renderer,
    font_index,
    "data/button.png",
    "data/button_pressed.png",
    "Open Shop",
    15,
    15,
    250,
    30);
}

UIElement::MenuItem
GameScreen::update(const ASGE::Point2D& cursor_pos, bool click)
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
  }

  return shop.update(cursor_pos, click);
}

void GameScreen::render(ASGE::Renderer* renderer, const int& currency)
{
  renderer->renderText("GAME", 300, 300, ASGE::COLOURS::WHITE);
  renderer->renderText(
    "Currency: " + std::to_string(currency), 300, 35, ASGE::COLOURS::WHITE);

  open_shop.render(renderer);

  if (shop_active)
  {
    shop.render(renderer);
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
