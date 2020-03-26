//
// Created by z2-rowbotham on 13/03/2020.
//

#ifndef MYNETGAME_TEXTBOX_H
#define MYNETGAME_TEXTBOX_H

#include "UIElement.h"

#include <Engine/Renderer.h>

class TextBox : public UIElement
{
 public:
  TextBox() = default;
  ~TextBox() override;

  TextBox(const TextBox& text){};           // Copy Constructor
  TextBox& operator=(const TextBox& text);  // Copy Assignment Operator

  bool init(
    ASGE::Renderer* renderer,
    int font_index,
    const std::string& normal_texture,
    const std::string& active_texture,
    const std::string& default_text,
    float x_pos,
    float y_pos,
    float width,
    float height);
  void update(ASGE::Point2D cursor_pos, bool click, std::atomic<bool>& key_press, int key);
  void render(ASGE::Renderer* renderer);

  void keyHandler(int key);

  std::string getText();

 private:
  ASGE::Sprite* normal_sprite = nullptr;
  ASGE::Sprite* active_sprite = nullptr;
  ASGE::Text text;

  bool active = false;

  const float BORDER_AMOUNT = 5;
};

#endif  // MYNETGAME_TEXTBOX_H
