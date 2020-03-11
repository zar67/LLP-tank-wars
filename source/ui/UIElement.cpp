//
// Created by z2-rowbotham on 11/03/2020.
//

#include "ui/UIElement.h"

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
