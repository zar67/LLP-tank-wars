//
// Created by z2-rowbotham on 11/03/2020.
//

#include "ui/scenes/Lobby.h"

Lobby::~Lobby()
{
  for (ASGE::Sprite* sprite : player_icons)
  {
    delete sprite;
  }
  player_icons.clear();
}

Lobby& Lobby::operator=(const Lobby& lobby)
{
  if (&lobby != this)
  {
    for (ASGE::Sprite* sprite : player_icons)
    {
      delete sprite;
    }
    player_icons.clear();
    player_icons = lobby.player_icons;
  }

  return *this;
}

bool Lobby::init(ASGE::Renderer* renderer, int font_index)
{
  lobby_title = UIElement::setupText(
    renderer,
    font_index,
    "Lobby",
    static_cast<float>(ASGE::SETTINGS.window_width) / 2,
    150,
    true,
    false,
    ASGE::COLOURS::BLACK,
    1,
    1.5F);

  return start_game.init(
    renderer,
    font_index,
    "data/button.png",
    "data/button_pressed.png",
    "Start Game",
    static_cast<float>(ASGE::SETTINGS.window_width) / 2 - 150,
    420,
    300,
    40);
}

UIElement::MenuItem Lobby::update(const ASGE::Point2D& cursor_pos, bool click)
{
  start_game.update(cursor_pos, click);

  if (start_game.pressed() && player_number > 0)
  {
    return UIElement::MenuItem::START_GAME;
  }

  return UIElement::MenuItem::NONE;
}

void Lobby::render(ASGE::Renderer* renderer)
{
  renderer->renderText(lobby_title);

  for (ASGE::Sprite* sprite : player_icons)
  {
    renderer->renderSprite(*sprite);
  }

  start_game.render(renderer);
}

bool Lobby::addPlayer(ASGE::Renderer* renderer)
{
  int x_pos = 0;
  if (player_number == 0)
  {
    x_pos = ASGE::SETTINGS.window_width / 2 - 25;
  }
  else
  {
    x_pos =
      ASGE::SETTINGS.window_width / 2 - ((60 * (player_number + 1) - 10) / 2);
  }

  ASGE::Sprite* sprite = renderer->createRawSprite();
  if (!UIElement::setupSprite(
        *sprite, "data/text_box.png", static_cast<float>(x_pos), 220, 50, 50))
  {
    return false;
  }

  player_number += 1;
  player_icons.push_back(sprite);

  int count = 0;
  for (ASGE::Sprite* icon : player_icons)
  {
    icon->xPos(static_cast<float>(x_pos + (60 * count)));
    count += 1;
  }
}
