//
// Created by z2-rowbotham on 11/03/2020.
//

#include "ui/scenes/MainMenu.h"

bool MainMenu::init(ASGE::Renderer* renderer, int font_index)
{
  menu_title = UIElement::setupText(
    renderer,
    font_index,
    "Main Menu",
    static_cast<float>(ASGE::SETTINGS.window_width) / 2,
    150,
    true,
    false,
    ASGE::COLOURS::BLACK,
    1,
    1.5F);

  if (!host_game.init(
        renderer,
        font_index,
        "data/sprites/ui/button.png",
        "data/sprites/ui/button_pressed.png",
        "Host",
        static_cast<float>(ASGE::SETTINGS.window_width) / 2 - 70,
        280,
        140,
        40))
  {
    return false;
  }

  if (!join_game.init(
        renderer,
        font_index,
        "data/sprites/ui/button.png",
        "data/sprites/ui/button_pressed.png",
        "Join",
        static_cast<float>(ASGE::SETTINGS.window_width) / 2 - 70,
        360,
        140,
        40))
  {
    return false;
  }

  return exit_game.init(
    renderer,
    font_index,
    "data/sprites/ui/button.png",
    "data/sprites/ui/button_pressed.png",
    "Exit",
    static_cast<float>(ASGE::SETTINGS.window_width) / 2 - 70,
    440,
    140,
    40);
}

UIElement::MenuItem MainMenu::update(const ASGE::Point2D& cursor_pos, bool click)
{
  host_game.update(cursor_pos, click);
  join_game.update(cursor_pos, click);
  exit_game.update(cursor_pos, click);

  if (host_game.pressed())
  {
    return UIElement::MenuItem::HOST_GAME;
  }

  if (join_game.pressed())
  {
    return UIElement::MenuItem::JOIN_SCREEN;
  }

  if (exit_game.pressed())
  {
    return UIElement::MenuItem::EXIT_GAME;
  }

  return UIElement::MenuItem::NONE;
}

void MainMenu::render(ASGE::Renderer* renderer)
{
  renderer->renderText(menu_title);
  host_game.render(renderer);
  join_game.render(renderer);
  exit_game.render(renderer);
}
