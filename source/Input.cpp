//
// Created by User on 26/03/2020.
//

#include "Input.h"
Input::Input(ASGE::Input& _inputs)
{
  key_callback_id = _inputs.addCallbackFnc(ASGE::E_KEY, &Input::keyHandler, this);

  move_callback_id = _inputs.addCallbackFnc(ASGE::E_MOUSE_MOVE, &Input::moveHandler, this);

  click_callback_id = _inputs.addCallbackFnc(ASGE::E_MOUSE_CLICK, &Input::clickHandler, this);

  std::thread input_thread(&Input::executeQueue, this);
  input_thread.detach();
  asge_input = &_inputs;
}

Input::~Input()
{
  if (asge_input != nullptr)
  {
    asge_input->unregisterCallback(static_cast<unsigned int>(key_callback_id));
    asge_input = nullptr;
  }
}

Input::Input(const Input& _input) {}

Input& Input::operator=(const Input& _input)
{
  if (&_input != this)
  {
    // add pointers in here
  }
  return *this;
}

/// Processes key inputs.
/// This function is added as a callback to handle the game's
/// keyboard input. For this game, calls to this function
/// are not thread safe, so you may alter the game's state
/// but your code needs to designed to prevent data-races.
/// @param data
/// @see KeyEvent

// make these call backs on threads
void Input::keyHandler(ASGE::SharedEventData data)
{
  eventInput(data, ASGE::E_KEY);
}

void Input::moveHandler(ASGE::SharedEventData data)
{
  eventInput(data, ASGE::E_MOUSE_MOVE);
}

void Input::clickHandler(ASGE::SharedEventData data)
{
  eventInput(data, ASGE::E_MOUSE_CLICK);
}

// called in thread
void Input::keyBoard(ASGE::SharedEventData data)
{
  const auto* key = dynamic_cast<const ASGE::KeyEvent*>(data.get());

  if (key->key == ASGE::KEYS::KEY_ESCAPE)
  {
    exitInputThread();
    // quit game
  }

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
void Input::mouse(ASGE::SharedEventData data)
{
  const auto* click = dynamic_cast<const ASGE::ClickEvent*>(data.get());
  mouse_click       = click->action == ASGE::MOUSE::BUTTON_PRESSED;
  if (!mouseClicked())
  {
    return;
  }
  for (auto& tile : map)
  {
    int tile_id = tile.mouseClicked(mousePos().x, mousePos().y);
    if (tile_id != 0)
    {
      if (tile_clicked != nullptr)
      {
        tile_clicked->sprite->colour(tile.sprite->colour());
      }
      mutex_tile_clicked.lock();
      tile_clicked = &tile;
      if (tile.troop_id > 0)
      {
        tile_clicked->sprite->colour(cant_click_col);
      }
      else
      {
        tile_clicked->sprite->colour(clicked_col);
      }
      mutex_tile_clicked.unlock();
      clicked_map = false;
      break;
    }
  }
}

void Input::eventInput(ASGE::SharedEventData s_data, ASGE::EventType e_data)
{
  InputData i_data       = InputData();
  i_data.sharedEventData = s_data;
  i_data.eventType       = e_data;
  input_queue.push(i_data);
}

std::queue<InputData>* Input::getInputQueue()
{
  mutex_queue.lock();
  return &input_queue;
}

/*
 * poll in thread
 * get items off queue
 */
void Input::executeQueue()
{
  while (is_active)
  {
    std::queue<InputData>* queue = getInputQueue();
    if (queue->empty())
    {
      mutex_queue.unlock();
    }
    else
    {
      InputData data = queue->front();
      queue->pop();
      executeEvent(data);
      mutex_queue.unlock();
    }
  }
}

void Input::exitInputThread()
{
  is_active = false;
}

// switch statement for each event type
void Input::executeEvent(const InputData& data)
{
  switch (data.eventType)
  {
  case ASGE::EventType::E_KEY:
  {
    keyBoard(data.sharedEventData);
    break;
  }
  case ASGE::EventType::E_MOUSE_CLICK:
  {
    mouse(data.sharedEventData);
    break;
  }
  case ASGE::EventType ::E_MOUSE_MOVE:
  {
    const auto* move = dynamic_cast<const ASGE::MoveEvent*>(data.sharedEventData.get());
    mouse_pos.x      = static_cast<float>(move->xpos);
    mouse_pos.y      = static_cast<float>(move->ypos);
    break;
  }
  }
}

TileData* Input::tileClicked()
{
  mutex_tile_clicked.lock();
  return tile_clicked;
}

void Input::unlockTile()
{
  mutex_tile_clicked.unlock();
}

void Input::setMap(const std::vector<TileData>& _map)
{
  map = _map;
}

void Input::setClickedMap(bool _map_clicked, float x, float y)
{
  clicked_map        = _map_clicked;
  recent_mouse_pos.x = x;
  recent_mouse_pos.y = y;
}
