//
// Created by z2-rowbotham on 11/03/2020.
//

#include "ui/SceneManager.h"

bool SceneManager::init(
  ASGE::Renderer* renderer, int font_index, int game_width, int game_height)
{
  if (!main_menu.init(renderer, font_index, game_width))
  {
    return false;
  }

  if (!lobby.init(renderer, font_index, game_width))
  {
    return false;
  }

  if (!game_screen.init(
        renderer,
        font_index,
        game_width,
        std::vector<std::string>{ "data/text_box.png",
                                  "data/text_box.png",
                                  "data/text_box.png",
                                  "data/text_box.png" }))
  {
    return false;
  }

  lobby.addPlayer(renderer, game_width);
  lobby.addPlayer(renderer, game_width);
  lobby.addPlayer(renderer, game_width);

  return true;
}

UIElement::MenuItem
SceneManager::update(const ASGE::Point2D& cursor_pos, bool click)
{
  UIElement::MenuItem item;
  switch (screen_open)
  {
    case Screens::MAIN_MENU:
    {
      item = main_menu.update(cursor_pos, click);
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
    default:
      item = UIElement::MenuItem::NONE;
      break;
  }

  switch (item)
  {
    case UIElement::MenuItem::OPEN_LOBBY:
    {
      screen_open = Screens::LOBBY;
      break;
    }
    case UIElement::MenuItem::START_GAME:
    {
      screen_open = Screens::GAME;
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

void SceneManager::openShop()
{
  game_screen.openShop();
}

void SceneManager::closeShop()
{
  game_screen.closeShop();
}
