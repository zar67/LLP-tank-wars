//
// Created by z2-rowbotham on 11/03/2020.
//

#include "ui/scenes/Lobby.h"

#include <Engine/Logger.hpp>

Lobby::~Lobby()
{
  for (ASGE::Sprite* sprite : player_icons) { delete sprite; }
  player_icons.clear();
}

Lobby& Lobby::operator=(const Lobby& lobby)
{
  if (&lobby != this)
  {
    for (ASGE::Sprite* sprite : player_icons) { delete sprite; }
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

  for (int i = 0; i < 4; i++) { addPlayer(renderer); }

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

  if (start_game.pressed() && !player_icons.empty())
  {
    return UIElement::MenuItem::START_GAME;
  }

  return UIElement::MenuItem::NONE;
}

void Lobby::render(ASGE::Renderer* renderer)
{
  renderer->renderText(lobby_title);

  for (int i = 0; i < player_number; i++) { renderer->renderSprite(*player_icons[i]); }

  start_game.render(renderer);
}

void Lobby::setPlayerNumber(int number)
{
  player_number = number;
  float x_pos   = static_cast<float>(ASGE::SETTINGS.window_width) / 2 - 25 -
                ((static_cast<float>(player_number) - 1) * 30);

  for (int i = 0; i < player_number; i++)
  { player_icons[i]->xPos(x_pos + (static_cast<float>(i * 60))); }
}

int Lobby::getPlayerNumber()
{
  return player_number;
}

void Lobby::addPlayer(ASGE::Renderer* renderer)
{
  ASGE::Sprite* sprite = renderer->createRawSprite();
  if (!sprite->loadTexture("data/text_box.png"))
  {
    Logging::log("*** COULDN'T ADD PLAYER ***");
  }

  sprite->width(50);
  sprite->height(50);
  sprite->xPos(0);
  sprite->yPos(220);

  player_icons.push_back(sprite);
}