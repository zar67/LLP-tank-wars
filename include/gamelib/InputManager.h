//
// Created by User on 26/03/2020.
//

#ifndef MYNETGAME_INPUTMANAGER_H
#define MYNETGAME_INPUTMANAGER_H

#include "InputData.h"
#include "Map/Map.h"
#include "Map/TileData.h"
#include "Troop.h"
#include "components/GameComponent.hpp"
#include "gamedata/DataStructs.h"

#include <Engine/Camera2D.hpp>
#include <Engine/OGLGame.h>
#include <mutex>
#include <queue>
#include <thread>

class InputManager
{
 public:
  explicit InputManager(ASGE::Input& _inputs, ASGE::Camera2D* camera2D, Map* game_map);

  ~InputManager();
  InputManager(const InputManager& _input);
  InputManager& operator=(const InputManager& _input);

  void keyHandler(ASGE::SharedEventData data);
  void moveHandler(ASGE::SharedEventData data);
  void clickHandler(ASGE::SharedEventData data);

  std::atomic<bool>* mouseClicked() { return &mouse_click; }
  ASGE::Point2D mousePos() { return mouse_pos; }
  int keyValue() { return key_value; }
  std::atomic<bool>* keyPressed() { return &key_pressed; }
  void setInGame(bool value);
  bool getIsCamFree();

  void eventInput(ASGE::SharedEventData data, ASGE::EventType e_data);
  std::queue<InputData>* getInputQueue();
  void executeQueue();
  void exitInputThread();

  TileData* tileClicked();
  void unlockTile();
  TileData* previousTileClicked();
  void unlockPreviousTile();

  void setClickedMap(
    int player_id,
    const std::vector<Troop*>& troops,
    bool _map_clicked,
    float x,
    float y);
  bool getClickedMap() { return clicked_map; }
  void deselectTile();
  void resetMapColours();
  void setBaseColours(int player_index);
  Troop* getTroop(std::vector<Troop*> troops, int id);

 private:
  void executeEvent(const InputData& data);
  void keyBoard(ASGE::SharedEventData data);
  void mouse(ASGE::SharedEventData data);
  void scrollMap(const ASGE::KeyEvent& key_event);

  ASGE::Point2D mouse_pos       = ASGE::Point2D(0, 0);
  std::atomic<bool> mouse_click = false;
  std::atomic<bool> key_pressed = false;
  std::atomic<int> key_value    = 0;

  int key_callback_id   = -1; /**< Key InputManager Callback ID. */
  int move_callback_id  = -1; /**< Key InputManager Callback ID. */
  int click_callback_id = -1; /**< Key InputManager Callback ID. */

  std::atomic<bool> clicked_map = true;
  std::atomic<bool> is_active   = true;
  std::atomic<bool> in_game     = false;
  std::mutex mutex_queue;
  std::queue<InputData> input_queue;
  ASGE::Input* asge_input = nullptr;

  std::mutex mutex_tile_clicked;
  TileData* tile_clicked = nullptr;
  std::mutex mutex_prev_tile_clicked;
  TileData* prev_tile_clicked = nullptr;

  ASGE::Point2D recent_mouse_pos = ASGE::Point2D(0, 0);

  ASGE::Colour clicked_col    = ASGE::COLOURS::DARKGREEN;
  ASGE::Colour cant_click_col = ASGE::COLOURS::RED;

  ASGE::Camera2D* cam_ref        = nullptr;
  std::atomic<bool> is_cam_free  = true;
  const float translate_distance = 200.0F;

  Map* map = nullptr;
};

#endif  // MYNETGAME_INPUTMANAGER_H
