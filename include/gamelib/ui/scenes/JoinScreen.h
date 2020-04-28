//
// Created by z2-rowbotham on 13/03/2020.
//

#ifndef MYNETGAME_JOINSCREEN_H
#define MYNETGAME_JOINSCREEN_H

#include "../../Audio/AudioManager.h"
#include "../ui_elements/Button.h"
#include "../ui_elements/TextBox.h"
#include "../ui_elements/UIElement.h"

#include <Engine/Renderer.h>

class JoinScreen
{
 public:
  JoinScreen()  = default;
  ~JoinScreen() = default;

  bool init(ASGE::Renderer* renderer, int font_index);
  UIElement::MenuItem update(
    AudioManager* audio_manager,
    const ASGE::Point2D& cursor_pos,
    bool click,
    std::atomic<bool>& key_pressed,
    int key);
  void render(ASGE::Renderer* renderer);

  void displayConnectionError();
  std::string getIP();

 private:
  ASGE::Text title;
  TextBox text_box;
  ASGE::Text error_message;
  Button start_game;

  bool display_error = false;
};

#endif  // MYNETGAME_JOINSCREEN_H
