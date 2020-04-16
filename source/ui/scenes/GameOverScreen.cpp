//
// Created by zar67 on 15/04/2020.
//

#include "ui/scenes/GameOverScreen.h"

bool GameOverScreen::init(ASGE::Renderer* renderer, int font_index)
{
  title = UIElement::setupText(
    renderer,
    font_index,
    "Game Over",
    static_cast<float>(ASGE::SETTINGS.window_width) / 2,
    150,
    true,
    false,
    ASGE::COLOURS::BLACK,
    1,
    1.5F);

  if (!main_menu.init(
        renderer,
        font_index,
        "data/sprites/ui/button.png",
        "data/sprites/ui/button_pressed.png",
        "Main Menu",
        static_cast<float>(ASGE::SETTINGS.window_width) / 2 - 125,
        580,
        250,
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
    static_cast<float>(ASGE::SETTINGS.window_width) / 2 - 125,
    640,
    250,
    40);
}

UIElement::MenuItem
GameOverScreen::update(const ASGE::Point2D& cursor_pos, std::atomic<bool>& click)
{
  main_menu.update(cursor_pos, click);
  if (main_menu.pressed())
  {
    click = false;
    return UIElement::MenuItem::OPEN_MENU;
  }

  exit_game.update(cursor_pos, click);
  if (exit_game.pressed())
  {
    click = false;
    return UIElement::MenuItem::EXIT_GAME;
  }

  return UIElement::MenuItem::NONE;
}

void GameOverScreen::render(ASGE::Renderer* renderer)
{
  renderer->renderText(title);
  renderer->renderText(
    "Player " + std::to_string(winning_player) + " wins!",
    ASGE::SETTINGS.window_width / 2 - 150,
    380);
  main_menu.render(renderer);
  exit_game.render(renderer);
}

void GameOverScreen::setWinningValues(int player_id)
{
  winning_player = player_id;
}
