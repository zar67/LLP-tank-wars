//
// Created by User on 26/03/2020.
//

#ifndef MYNETGAME_INPUT_H
#define MYNETGAME_INPUT_H

#include "Engine/Input.h"

#include <Engine/OGLGame.h>

class Input
{
 public:
  explicit Input(ASGE::Input& _inputs);
  ~Input() = default;

  void keyHandler(ASGE::SharedEventData data);
  void moveHandler(ASGE::SharedEventData data);
  void clickHandler(ASGE::SharedEventData data);

 private:
  ASGE::Point2D mouse_pos = ASGE::Point2D(0, 0);
  bool mouse_click        = false;
  bool key_pressed        = false;
  int key_value           = 0;

  int key_callback_id   = -1; /**< Key Input Callback ID. */
  int move_callback_id  = -1; /**< Key Input Callback ID. */
  int click_callback_id = -1; /**< Key Input Callback ID. */
};

#endif  // MYNETGAME_INPUT_H
