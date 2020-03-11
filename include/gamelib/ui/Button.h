//
// Created by z2-rowbotham on 11/03/2020.
//

#ifndef MYNETGAME_BUTTON_H
#define MYNETGAME_BUTTON_H

#include "UIElement.h"
#include <Engine/Text.hpp>

class Button : public UIElement
{
 public:
  enum ButtonState
  {
    NONE,
    PRESSED,
    HOVER
  };

 public:
  Button() = default;
  ~Button() override;

  Button(const Button& button){};          // Copy Constructor
  Button& operator=(const Button& button); // Copy Assignment Operator

  bool init(
    ASGE::Renderer* renderer, const std::string& normal_texture,
    const std::string& pressed_texture, const std::string& button_text,
    float x_pos, float y_pos, float width, float height);
  void update(const ASGE::Point2D& cursor_pos, bool click);
  void render(ASGE::Renderer* renderer);

  ASGE::Sprite* getSprite();

  bool pressed();
  bool held();
  bool hover();

 private:
  ASGE::Sprite* normal_sprite  = nullptr;
  ASGE::Sprite* pressed_sprite = nullptr;
  std::string text             = "";

  ASGE::Point2D text_pos;
  ButtonState previous_state = NONE;
  ButtonState state          = NONE;
};

#endif // MYNETGAME_BUTTON_H
