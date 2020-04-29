//
// Created by z2-rowbotham on 11/03/2020.
//

#ifndef MYNETGAME_UIELEMENT_H
#define MYNETGAME_UIELEMENT_H

#include <Engine/Colours.h>
#include <Engine/Sprite.h>
#include <Engine/Text.hpp>

class UIElement
{
 public:
  enum class MenuItem
  {
    NONE            = -1,
    HOST_GAME       = 1,
    JOIN_SCREEN     = 2,
    CONNECT_TO_IP   = 3,
    EXIT_GAME       = 4,
    OPEN_MENU       = 5,
    START_GAME      = 6,
    END_TURN        = 7,
    SHOP_BUTTON     = 8,
    BUY_NORMAL_TANK = 9,
    BUY_BIG_TANK    = 10,
    BUY_LARGE_TANK  = 11,
    BUY_HUGE_TANK   = 12,
    BUY_UNIT_4      = 13,
    MAP_CLICK       = 14
  };

 public:
  explicit UIElement() = default;
  virtual ~UIElement() = default;

  static bool isInside(const ASGE::Point2D& point, ASGE::Sprite* sprite);
  static bool setupSprite(
    ASGE::Sprite& sprite,
    const std::string& texture,
    float x_pos,
    float y_pos,
    float width,
    float height,
    float opacity = 1);
  static ASGE::Text setupText(
    ASGE::Renderer* renderer,
    int font_index,
    const std::string& text,
    float x_pos,
    float y_pos,
    bool center_x,
    bool center_y,
    ASGE::Colour colour,
    short z_order,
    float scale = 1);
};

#endif  // MYNETGAME_UIELEMENT_H
