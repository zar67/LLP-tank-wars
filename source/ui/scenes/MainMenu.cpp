//
// Created by z2-rowbotham on 11/03/2020.
//

#include "ui/scenes/MainMenu.h"

bool MainMenu::init(ASGE::Renderer* renderer, int font_index, int game_width)
{
  menu_title = UIElement::setupText(
    renderer,
    font_index,
    "Main Menu",
    static_cast<float>(game_width) / 2,
    150,
    true,
    false,
    ASGE::COLOURS::BLACK,
    1,
    1.5F);

  if (!start_game.init(
        renderer,
        font_index,
        "data/button.png",
        "data/button_pressed.png",
        "Start",
        static_cast<float>(game_width) / 2 - 50,
        280,
        140,
        40))
  {
    return false;
  }

  return exit_game.init(
    renderer,
    font_index,
    "data/button.png",
    "data/button_pressed.png",
    "Exit",
    static_cast<float>(game_width) / 2 - 50,
    360,
    140,
    40);
}

UIElement::MenuItem
MainMenu::update(const ASGE::Point2D& cursor_pos, bool click)
{
  start_game.update(cursor_pos, click);
  exit_game.update(cursor_pos, click);

  if (start_game.pressed())
  {
    return UIElement::MenuItem::OPEN_LOBBY;
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
  start_game.render(renderer);
  exit_game.render(renderer);
}
