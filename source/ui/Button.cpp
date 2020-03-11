//
// Created by z2-rowbotham on 11/03/2020.
//

#include "ui/Button.h"
#include <Engine/Renderer.h>

Button::~Button()
{
  delete normal_sprite;
  normal_sprite = nullptr;

  delete pressed_sprite;
  pressed_sprite = nullptr;
}

Button& Button::operator=(const Button& button)
{
  if (&button != this)
  {
    delete normal_sprite;
    this->normal_sprite = button.normal_sprite;

    delete pressed_sprite;
    this->pressed_sprite = button.pressed_sprite;
  }

  return *this;
}

bool Button::init(
  ASGE::Renderer* renderer, const std::string& normal_texture,
  const std::string& pressed_texture, const std::string& button_text,
  float x_pos, float y_pos, float width, float height)
{
  text = button_text;

  normal_sprite = renderer->createRawSprite();
  if (!setupSprite(
        *normal_sprite, normal_texture, x_pos, y_pos, width, height, 0.6F))
  {
    return false;
  }

  pressed_sprite = renderer->createRawSprite();
  if (!setupSprite(
        *pressed_sprite, pressed_texture, x_pos, y_pos, width, height))
  {
    return false;
  }

  text_pos.x = x_pos + (width / 2) - (static_cast<float>(text.length()) * 6);
  text_pos.y = y_pos + (height / 2) + 5;

  return true;
}

void Button::update(const ASGE::Point2D& cursor_pos, bool click)
{
  previous_state = state;
  if (isInside(cursor_pos, normal_sprite))
  {
    if (click)
    {
      state = ButtonState::PRESSED;
    }
    else
    {
      state = ButtonState::HOVER;
      normal_sprite->opacity(1.0F);
    }
  }
  else
  {
    state = ButtonState::NONE;
    normal_sprite->opacity(0.6F);
  }
}

void Button::render(ASGE::Renderer* renderer)
{
  if (state == ButtonState::PRESSED)
  {
    renderer->renderSprite(*pressed_sprite);
  }
  else
  {
    renderer->renderSprite(*normal_sprite);
  }

  // renderer->renderText(text, text_pos.x, text_pos.y, ASGE::COLOURS::BLACK);
}

ASGE::Sprite* Button::getSprite()
{
  return normal_sprite;
}

bool Button::pressed()
{
  return previous_state != PRESSED && state == PRESSED;
}

bool Button::held()
{
  return previous_state == PRESSED && state == PRESSED;
}

bool Button::hover()
{
  return state == HOVER;
}
