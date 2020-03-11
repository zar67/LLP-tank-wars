//
// Created by z2-rowbotham on 11/03/2020.
//

#include "ui/ui_elements/UIElement.h"
#include <Engine/Renderer.h>

bool UIElement::isInside(const ASGE::Point2D& point, ASGE::Sprite* sprite)
{
  auto bbox = sprite->getGlobalBounds();

  return point.x >= bbox.v1.x && point.x <= bbox.v2.x && point.y >= bbox.v1.y &&
         point.y <= bbox.v3.y;
}

bool UIElement::setupSprite(
  ASGE::Sprite& sprite, const std::string& texture, float x_pos, float y_pos,
  float width, float height, float opacity)
{
  if (!sprite.loadTexture(texture))
  {
    return false;
  }

  sprite.xPos(x_pos);
  sprite.yPos(y_pos);
  sprite.width(width);
  sprite.height(height);
  sprite.opacity(opacity);

  return true;
}

ASGE::Text UIElement::setupText(
  ASGE::Renderer* renderer, int font_index, const std::string& text,
  float x_pos, float y_pos, bool center_x, bool center_y, ASGE::Colour colour,
  short z_order, float scale)
{
  ASGE::Text text_obj = ASGE::Text(renderer->getFont(font_index), text);
  text_obj.setColour(colour);
  text_obj.setScale(scale);

  if (center_x)
  {
    text_obj.setPositionX(x_pos - (text_obj.getWidth() / 2));
  }
  else
  {
    text_obj.setPositionX(x_pos);
  }

  if (center_y)
  {
    text_obj.setPositionY(y_pos + (text_obj.getHeight() / 2));
  }
  else
  {
    text_obj.setPositionY(y_pos);
  }

  text_obj.setZOrder(z_order);

  return text_obj;
}
