//
// Created by z2-rowbotham on 11/03/2020.
//

#ifndef MYNETGAME_UIELEMENT_H
#define MYNETGAME_UIELEMENT_H

#include <Engine/Sprite.h>

class UIElement
{
 public:
  enum class MenuItem
  {
    NONE       = -1,
    OPEN_LOBBY = 1,
    EXIT_GAME  = 2,
    START_GAME = 3,
    OPEN_SHOP  = 4,
    HIDE_SHOP  = 5,
    BUY_UNIT_0,
    BUY_UNIT_1,
    BUY_UNIT_2,
    BUY_UNIT_3
  };

 public:
  explicit UIElement() = default;
  virtual ~UIElement() = default;

  static bool isInside(const ASGE::Point2D& point, ASGE::Sprite* sprite);
  static bool setupSprite(
    ASGE::Sprite& sprite, const std::string& texture, float x_pos, float y_pos,
    float width, float height, float opacity = 1);

 private:
};

#endif // MYNETGAME_UIELEMENT_H
