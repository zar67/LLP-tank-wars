//
// Created by User on 26/03/2020.
//

#include "Input.h"
Input::Input(ASGE::Input& _inputs)
{
  key_callback_id = _inputs.addCallbackFnc(ASGE::E_KEY, &Input::keyHandler, this);

  move_callback_id = _inputs.addCallbackFnc(ASGE::E_MOUSE_MOVE, &Input::moveHandler, this);

  click_callback_id = _inputs.addCallbackFnc(ASGE::E_MOUSE_CLICK, &Input::clickHandler, this);
}
// Input::Input()
//{
// key_callback_id = ASGE::In::addCallbackFnc(ASGE::E_KEY, &Input::keyHandler, this);

//  move_callback_id = inputs->addCallbackFnc(ASGE::E_MOUSE_MOVE, &Input::moveHandler, this);

// click_callback_id = inputs->addCallbackFnc(ASGE::E_MOUSE_CLICK, &Input::clickHandler, this);
//}
/// Processes key inputs.
/// This function is added as a callback to handle the game's
/// keyboard input. For this game, calls to this function
/// are not thread safe, so you may alter the game's state
/// but your code needs to designed to prevent data-races.
/// @param data
/// @see KeyEvent
void Input::keyHandler(ASGE::SharedEventData data)
{
  const auto* key = dynamic_cast<const ASGE::KeyEvent*>(data.get());

  if (key->key == ASGE::KEYS::KEY_ESCAPE) {}

  if (key->action == ASGE::KEYS::KEY_PRESSED)
  {
    key_pressed = true;
    key_value   = key->key;
  }
  else if (key->action == ASGE::KEYS::KEY_RELEASED)
  {
    key_pressed = false;
  }
}

void Input::moveHandler(ASGE::SharedEventData data)
{
  const auto* move = dynamic_cast<const ASGE::MoveEvent*>(data.get());

  mouse_pos.x = static_cast<float>(move->xpos);
  mouse_pos.y = static_cast<float>(move->ypos);
}

void Input::clickHandler(ASGE::SharedEventData data)
{
  const auto* click = dynamic_cast<const ASGE::ClickEvent*>(data.get());

  mouse_click = click->action == ASGE::MOUSE::BUTTON_PRESSED;
}

// call this function in seperate thread
// polls for input
void Input::pollInput(ASGE::Input& _inputs) {}
