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
    std::vector<std::string>{
      "data/text_box.png", "data/text_box.png", "data/text_box.png", "data/text_box.png"});
}

UIElement::MenuItem
SceneManager::update(const ASGE::Point2D& cursor_pos, bool click, bool key_pressed, int key)
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
  case Screens::GAME:
  {
    game_screen.render(renderer, currency);
    break;
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
