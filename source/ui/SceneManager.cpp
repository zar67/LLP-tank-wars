//
// Created by z2-rowbotham on 11/03/2020.
//

#include "ui/SceneManager.h"

#include <Engine/Logger.hpp>

bool SceneManager::init(ASGE::Renderer* renderer, AudioManager* audio, int font_index)
{
  audio_manager = audio;
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

  if (!game_over.init(renderer, font_index))
  {
    return false;
  }

  return game_screen.init(renderer, font_index);
}

UIElement::MenuItem SceneManager::update(InputManager* input_manager, std::array<int, 2> cam_pos)
{
  UIElement::MenuItem item;

  switch (screen_open)
  {
  case Screens::MAIN_MENU:
  {
    item =
      main_menu.update(audio_manager, input_manager->mousePos(), *input_manager->mouseClicked());
    break;
  }
  case Screens::JOIN_SCREEN:
  {
    item = join_screen.update(
      audio_manager,
      input_manager->mousePos(),
      *input_manager->mouseClicked(),
      *input_manager->keyPressed(),
      input_manager->keyValue());
    break;
  }
  case Screens::LOBBY:
  {
    item = lobby.update(audio_manager, input_manager->mousePos(), *input_manager->mouseClicked());
    break;
  }
  case Screens::GAME:
  {
    item = game_screen.update(
      audio_manager, input_manager->mousePos(), *input_manager->mouseClicked(), cam_pos);
    break;
  }
  case Screens::GAME_OVER:
  {
    item =
      game_over.update(audio_manager, input_manager->mousePos(), *input_manager->mouseClicked());
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
  bool alive,
  Troop* troop_selected,
  const std::vector<std::vector<Troop*>>& troops,
  Map* map,
  int currency)
{
  game_screen.render(
    renderer, action_number, current_player_turn, in_turn, alive, troop_selected, currency);

  map->renderMap(renderer);

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

GameOverScreen* SceneManager::gameOverScreen()
{
  return &game_over;
}
