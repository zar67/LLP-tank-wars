//
// Created by User on 26/03/2020.
//

#include "InputManager.h"
InputManager::InputManager(ASGE::Input& _inputs, AudioManager* audio, Map* game_map)
{
  key_callback_id = _inputs.addCallbackFnc(ASGE::E_KEY, &InputManager::keyHandler, this);

  move_callback_id = _inputs.addCallbackFnc(ASGE::E_MOUSE_MOVE, &InputManager::moveHandler, this);

  click_callback_id =
    _inputs.addCallbackFnc(ASGE::E_MOUSE_CLICK, &InputManager::clickHandler, this);

  std::thread input_thread(&InputManager::executeQueue, this);
  input_thread.detach();
  asge_input = &_inputs;

  audio_manager = audio;
  map           = game_map;
}

InputManager::~InputManager()
{
  exitInputThread();
  if (asge_input != nullptr)
  {
    asge_input->unregisterCallback(static_cast<unsigned int>(key_callback_id));
    asge_input = nullptr;
  }
  tile_clicked = nullptr;
}

InputManager::InputManager(const InputManager& _input) {}

InputManager& InputManager::operator=(const InputManager& _input)
{
  if (&_input != this)
  {
    // add pointers in here
    delete map;
    this->map = _input.map;
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
void InputManager::keyHandler(ASGE::SharedEventData data)
{
  eventInput(data, ASGE::E_KEY);
}

void InputManager::moveHandler(ASGE::SharedEventData data)
{
  eventInput(data, ASGE::E_MOUSE_MOVE);
}

void InputManager::clickHandler(ASGE::SharedEventData data)
{
  eventInput(data, ASGE::E_MOUSE_CLICK);
}

// called in thread
void InputManager::keyBoard(ASGE::SharedEventData data)
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
void InputManager::mouse(ASGE::SharedEventData data)
{
  const auto* click = dynamic_cast<const ASGE::ClickEvent*>(data.get());
  mouse_click       = click->action == ASGE::MOUSE::BUTTON_PRESSED &&
                click->button == ASGE::MOUSE::MOUSE_BTN1;

  if (
    click->action == ASGE::MOUSE::BUTTON_PRESSED && click->button == ASGE::MOUSE::MOUSE_BTN2 &&
    tile_clicked != nullptr)
  {
    audio_manager->playDeselection();
    deselectTile();
  }
}

void InputManager::eventInput(ASGE::SharedEventData s_data, ASGE::EventType e_data)
{
  InputData i_data       = InputData();
  i_data.sharedEventData = s_data;
  i_data.eventType       = e_data;
  input_queue.push(i_data);
}

std::queue<InputData>* InputManager::getInputQueue()
{
  mutex_queue.lock();
  return &input_queue;
}

/*
 * poll in thread
 * get items off queue
 */
void InputManager::executeQueue()
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

void InputManager::exitInputThread()
{
  is_active = false;
}

// switch statement for each event type
void InputManager::executeEvent(const InputData& data)
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

TileData* InputManager::tileClicked()
{
  mutex_tile_clicked.lock();
  return tile_clicked;
}

void InputManager::unlockTile()
{
  mutex_tile_clicked.unlock();
}

TileData* InputManager::previousTileClicked()
{
  mutex_prev_tile_clicked.lock();
  return prev_tile_clicked;
}

void InputManager::unlockPreviousTile()
{
  mutex_prev_tile_clicked.unlock();
}

void InputManager::setClickedMap(
  int player_id,
  const std::vector<Troop*>& troops,
  bool _map_clicked,
  float x,
  float y)
{
  resetMapColours();
  prev_tile_clicked = tile_clicked;

  std::vector<TileData>* tiles = map->getMap();
  clicked_map                  = _map_clicked;
  recent_mouse_pos.x           = x;
  recent_mouse_pos.y           = y;

  for (auto& tile : *tiles)
  {
    int tile_id = tile.mouseClicked(x, y);
    if (tile_id >= 0)
    {
      if (tile.is_base)
      {
        continue;
      }
      if (tile_clicked != nullptr)
      {
        tile_clicked->sprite->colour(tile.sprite->colour());
      }
      mutex_tile_clicked.lock();
      tile_clicked = &tile;
      if (tile.troop_id >= 0)
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

  if (
    tile_clicked != nullptr && tile_clicked->troop_id != -1 &&
    tile_clicked->troop_player_id == player_id)
  {
    for (auto& tile : *tiles)
    {
      if (
        map->tileInRange(
          tile.tile_id,
          tile_clicked->tile_id,
          getTroop(troops, tile_clicked->troop_id)->getWeaponRange()) &&
        tile.troop_id != -1 && tile.troop_player_id != player_id)
      {
        tile.sprite->colour(ASGE::COLOURS::GREEN);
      }
      else if (
        map->tileInRange(
          tile.tile_id,
          tile_clicked->tile_id,
          getTroop(troops, tile_clicked->troop_id)->getMovementRange()) &&
        !tile.is_base)
      {
        tile.sprite->colour(cant_click_col);
      }
    }
  }
}

void InputManager::deselectTile()
{
  if (tile_clicked != nullptr)
  {
    tile_clicked      = nullptr;
    prev_tile_clicked = nullptr;
  }

  resetMapColours();
}

void InputManager::resetMapColours()
{
  std::vector<TileData>* tiles = map->getMap();
  for (auto& tile : *tiles)
  {
    if (tile.is_base)
    {
      tile.sprite->colour(ASGE::COLOURS::BLUE);
    }
    else
    {
      tile.sprite->colour(ASGE::COLOURS::WHITE);
    }
  }
}

Troop* InputManager::getTroop(std::vector<Troop*> troops, int id)
{
  auto it = std::find_if(troops.begin(), troops.end(), [id](const Troop* troop) {
    return troop->getID() == id;
  });

  if (it != troops.end())
  {
    return *it;
  }
}
