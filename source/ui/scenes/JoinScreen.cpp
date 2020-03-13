//
// Created by z2-rowbotham on 13/03/2020.
//

#include "ui/scenes/JoinScreen.h"

bool JoinScreen::init(ASGE::Renderer* renderer, int font_index)
{
  title = UIElement::setupText(
    renderer,
    font_index,
    "Enter IP To Connet To: ",
    static_cast<float>(ASGE::SETTINGS.window_width) / 2,
    150,
    true,
    false,
    ASGE::COLOURS::BLACK,
    1);

  if (!text_box.init(
        renderer,
        font_index,
        "data/text_box.png",
        "data/button.png",
        "",
        static_cast<float>(ASGE::SETTINGS.window_width) / 2 - 200,
        240,
        400,
        50))
  {
    return false;
  }

  error_message = UIElement::setupText(
    renderer,
    font_index,
    "Could Not Connect To IP",
    static_cast<float>(ASGE::SETTINGS.window_width) / 2,
    350,
    true,
    false,
    ASGE::COLOURS::RED,
    1);

  return start_game.init(
    renderer,
    font_index,
    "data/button.png",
    "data/button_pressed.png",
    "Join",
    static_cast<float>(ASGE::SETTINGS.window_width) / 2 - 125,
    500,
    250,
    40);
}

UIElement::MenuItem JoinScreen::update(
  const ASGE::Point2D& cursor_pos, bool click, bool key_pressed, int key)
{
  start_game.update(cursor_pos, click);
  text_box.update(cursor_pos, click, key_pressed, key);

  if (start_game.pressed())
  {
    return UIElement::MenuItem::CONNECT_TO_IP;
  }

  return UIElement::MenuItem::NONE;
}

void JoinScreen::render(ASGE::Renderer* renderer)
{
  renderer->renderText(title);
  text_box.render(renderer);
  start_game.render(renderer);

  if (display_error)
  {
    renderer->renderText(error_message);
  }
}

void JoinScreen::displayConnectionError()
{
  display_error = true;
}

std::string JoinScreen::getIP()
{
  return text_box.getText();
}
