//
// Created by z2-rowbotham on 13/03/2020.
//

#include "ui/ui_elements/TextBox.h"

#include <Engine/Keys.h>

TextBox::~TextBox()
{
  delete normal_sprite;
  normal_sprite = nullptr;

  delete active_sprite;
  active_sprite = nullptr;
}

TextBox& TextBox::operator=(const TextBox& text)
{
  if (&text != this)
  {
    delete normal_sprite;
    this->normal_sprite = text.normal_sprite;

    delete active_sprite;
    this->active_sprite = text.active_sprite;
  }

  return *this;
}

bool TextBox::init(
  ASGE::Renderer* renderer,
  int font_index,
  const std::string& normal_texture,
  const std::string& active_texture,
  const std::string& default_text,
  float x_pos,
  float y_pos,
  float width,
  float height)
{
  float center_y =
    y_pos + (height / 2) + (static_cast<float>(renderer->getFont(font_index).line_height) / 4);
  text = setupText(
    renderer,
    font_index,
    default_text,
    x_pos + BORDER_AMOUNT,
    center_y,
    false,
    true,
    ASGE::COLOURS::BLACK,
    1);

  normal_sprite = renderer->createRawSprite();
  if (!setupSprite(*normal_sprite, normal_texture, x_pos, y_pos, width, height))
  {
    return false;
  }

  active_sprite = renderer->createRawSprite();
  return setupSprite(
    *active_sprite,
    active_texture,
    x_pos - BORDER_AMOUNT,
    y_pos - BORDER_AMOUNT,
    width + (BORDER_AMOUNT * 2),
    height + (BORDER_AMOUNT * 2));
}

void TextBox::update(ASGE::Point2D cursor_pos, bool click, std::atomic<bool>& key_press, int key)
{
  if (click)
  {
    active = isInside(cursor_pos, normal_sprite);
  }

  if (active && key_press)
  {
    keyHandler(key);
    key_press = false;
  }
}

void TextBox::render(ASGE::Renderer* renderer)
{
  if (active)
  {
    renderer->renderSprite(*active_sprite);
  }

  renderer->renderSprite(*normal_sprite);
  renderer->renderText(text);
}

void TextBox::keyHandler(int key)
{
  if (key >= ASGE::KEYS::KEY_SPACE && key <= ASGE::KEYS::KEY_GRAVE_ACCENT)
  {
    text.setString(text.getString() + (char)key);
  }
  else if (text.getString().length() > 0 && key == ASGE::KEYS::KEY_BACKSPACE)
  {
    std::string string = text.getString();
    string.pop_back();
    text.setString(string);
  }
  else if (key == ASGE::KEYS::KEY_ENTER)
  {
    active = false;
  }
}

std::string TextBox::getText()
{
  return text.getString();
}