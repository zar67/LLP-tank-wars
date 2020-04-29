//
// Created by z2-rowbotham on 11/03/2020.
//

#ifndef MYNETGAME_BUTTON_H
#define MYNETGAME_BUTTON_H

#include "UIElement.h"

#include <Engine/Renderer.h>
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

  Button(const Button& button){};           // Copy Constructor
  Button& operator=(const Button& button);  // Copy Assignment Operator

  bool init(
    ASGE::Renderer* renderer,
    int font_index,
    const std::string& normal_texture,
    const std::string& pressed_texture,
    const std::string& button_text,
    float x_pos,
    float y_pos,
    float width,
    float height);
  void update(const ASGE::Point2D& cursor_pos, bool click);
  void update(const ASGE::Point2D& cursor_pos, bool click, std::array<int, 2> cam_pos);
  void render(ASGE::Renderer* renderer);

  ASGE::Sprite* getSprite();
  void changeText(std::string new_text);

  bool pressed();
  bool held();
  bool hover();

 private:
  ASGE::Sprite* normal_sprite  = nullptr;
  ASGE::Sprite* pressed_sprite = nullptr;
  ASGE::Text text;

  ButtonState previous_state = NONE;
  ButtonState state          = NONE;

  const float BASE_OPACITY = 0.6F;

  std::array<int, 2> local_cam_pos = {0, 0};
};

#endif  // MYNETGAME_BUTTON_H
