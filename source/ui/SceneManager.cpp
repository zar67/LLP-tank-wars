//
// Created by z2-rowbotham on 11/03/2020.
//

#include "ui/SceneManager.h"

#include <Engine/Logger.hpp>

bool SceneManager::init(ASGE::Renderer* renderer, int font_index)
{
  if (!audio.audioSetUp())
  {
    return false;
  }

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


  /// start game menu music
  audio.playBackgroundMusic();

  if (!game_over.init(renderer, font_index))
  {
    return false;
  }

  return game_screen.init(renderer, font_index);
}

UIElement::MenuItem SceneManager::update(InputManager* input_manager)
{
  UIElement::MenuItem item;

  switch (screen_open)
  {
  case Screens::MAIN_MENU:
  {
    item = main_menu.update(input_manager->mousePos(), *input_manager->mouseClicked());
    break;
  }
  case Screens::JOIN_SCREEN:
  {
    item = join_screen.update(
      input_manager->mousePos(),
      *input_manager->mouseClicked(),
      *input_manager->keyPressed(),
      input_manager->keyValue());
    break;
  }
  case Screens::LOBBY:
  {
    item = lobby.update(input_manager->mousePos(), *input_manager->mouseClicked());
    break;
  }
  case Screens::GAME:
  {
    item = game_screen.update(input_manager->mousePos(), *input_manager->mouseClicked());
    break;
  }
  case Screens::GAME_OVER:
  {
    item = game_over.update(input_manager->mousePos(), *input_manager->mouseClicked());
    break;
  }
  default: item = UIElement::MenuItem::NONE; break;
  }

  switch (item)
  {
  case UIElement::MenuItem::OPEN_MENU:
  {
    screen_open = Screens::MAIN_MENU;
    break;
  }
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

void SceneManager::render(ASGE::Renderer* renderer)
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
  case Screens::GAME_OVER:
  {
    game_over.render(renderer);
    break;
  }
  }
}

void SceneManager::renderGameScreen(
  ASGE::Renderer* renderer,
  int action_number,
  int current_player_turn,
  bool in_turn,
  Troop* troop_selected,
  const std::vector<std::vector<Troop*>>& troops,
  const std::vector<TileData>& tile_data,
  int currency)
{
  game_screen.render(
    renderer, action_number, current_player_turn, in_turn, troop_selected, currency);

  for (const auto& tile : tile_data)
  {
    if (tile.sprite != nullptr)
    {
      renderer->renderSprite(*tile.sprite);
    }
  }

  for (const auto& player : troops)
  {
    for (Troop* troop : player)
    {
      if (troop->getSpriteComponent()->getSprite() != nullptr)
      {
        renderer->renderSprite(*troop->getSpriteComponent()->getSprite());
      }
    }
  }
}

void SceneManager::screenOpen(Screens screen)
{
  screen_open = screen;
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
