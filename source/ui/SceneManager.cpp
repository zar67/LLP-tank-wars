//
// Created by z2-rowbotham on 11/03/2020.
//

#include "ui/SceneManager.h"

bool SceneManager::init(ASGE::Renderer* renderer, int font_index)
{
  if (!main_menu.init(renderer, font_index))
  {
    return false;
  }

  if (!join_screen.init(renderer, font_index))
  {
    return false;
  }

  if (!lobby.init(renderer, font_index))
  {
    return false;
  }

  return game_screen.init(
    renderer,
    font_index,
    std::vector<std::string>{"data/sprites/troops/tank_blue.png",
                             "data/sprites/troops/tank_dark.png",
                             "data/sprites/troops/tank_green.png",
                             "data/sprites/troops/tank_red.png",
                             "data/sprites/troops/tank_sand.png"});
}

UIElement::MenuItem SceneManager::update(
  const ASGE::Point2D& cursor_pos,
  bool click,
  std::atomic<bool>& key_pressed,
  int key)
{
  UIElement::MenuItem item;
  switch (screen_open)
  {
  case Screens::MAIN_MENU:
  {
    item = main_menu.update(cursor_pos, click);
    break;
  }
  case Screens::JOIN_SCREEN:
  {
    item = join_screen.update(cursor_pos, click, key_pressed, key);
    break;
  }
  case Screens::LOBBY:
  {
    item = lobby.update(cursor_pos, click);
    break;
  }
  case Screens::GAME:
  {
    item = game_screen.update(cursor_pos, click);
    break;
  }
  default: item = UIElement::MenuItem::NONE; break;
  }

  switch (item)
  {
  case UIElement::MenuItem::HOST_GAME:
  {
    screen_open = Screens::LOBBY;
    break;
  }
  case UIElement::MenuItem::JOIN_SCREEN:
  {
    screen_open = Screens::JOIN_SCREEN;
    break;
  }
  case UIElement::MenuItem::BACK_TO_MENU:
  {
    screen_open = Screens::MAIN_MENU;
    break;
  }
  default:
  {
    break;
  }
  }

  return item;
}

void SceneManager::render(
  ASGE::Renderer* renderer,
  int current_player_turn,
  bool in_turn,
  const std::vector<std::vector<Troop>>& troops,
  const std::vector<TileData>& tile_data,
  int currency)
{
  switch (screen_open)
  {
  case Screens::MAIN_MENU:
  {
    main_menu.render(renderer);
    break;
  }
  case Screens::JOIN_SCREEN:
  {
    join_screen.render(renderer);
    break;
  }
  case Screens::LOBBY:
  {
    lobby.render(renderer);
    break;
  }
  case Screens::GAME:
  {
    renderGameScreen(renderer, current_player_turn, in_turn, troops, tile_data, currency);
    break;
  }
  }
}

void SceneManager::renderGameScreen(
  ASGE::Renderer* renderer,
  int current_player_turn,
  bool in_turn,
  const std::vector<std::vector<Troop>>& troops,
  const std::vector<TileData>& tile_data,
  int currency)
{
  game_screen.render(renderer, current_player_turn, in_turn, currency);

  for (auto& tile : tile_data)
  {
    if (tile.sprite != nullptr)
    {
      renderer->renderSprite(*tile.sprite);
    }
  }

  for (const auto& player : troops)
  {
    for (Troop troop : player)
    {
      if (troop.getSpriteComponent()->getSprite() != nullptr)
      {
        renderer->renderSprite(*troop.getSpriteComponent()->getSprite());
      }
    }
  }
}

void SceneManager::screenOpen(Screens screen)
{
  screen_open = screen;
}

bool SceneManager::inMenu()
{
  return false;
}

MainMenu* SceneManager::mainMenu()
{
  return &main_menu;
}

JoinScreen* SceneManager::joinScreen()
{
  return &join_screen;
}

Lobby* SceneManager::lobbyScreen()
{
  return &lobby;
}

GameScreen* SceneManager::gameScreen()
{
  return &game_screen;
}
