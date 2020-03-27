//
// Created by User on 26/03/2020.
//

#ifndef MYNETGAME_INPUT_H
#define MYNETGAME_INPUT_H

#include "InputData.h"
#include "components/GameComponent.hpp"
#include "gamedata/DataStructs.h"

#include <Engine/OGLGame.h>
#include <mutex>
#include <queue>
#include <thread>
class Input
{
 public:
  explicit Input(ASGE::Input& _inputs);
  ~Input();
  Input(const Input& _input);
  Input& operator=(const Input& _input);

  void keyHandler(ASGE::SharedEventData data);
  void moveHandler(ASGE::SharedEventData data);
  void clickHandler(ASGE::SharedEventData data);

  bool mouseClicked() { return mouse_click; }
  ASGE::Point2D mousePos() { return mouse_pos; }
  int keyValue() { return key_value; }
  std::atomic<bool>* keyPressed() { return &key_pressed; }

  void keyBoard(ASGE::SharedEventData data);

  void eventInput(ASGE::SharedEventData data, ASGE::EventType e_data);
  std::queue<InputData>* getInputQueue();
  void executeQueue();
  void exitInputThread();

  void setMap(const std::vector<TileData>& _map);
  TileData* tileClicked();
  void unlockTile();

 private:
  void executeEvent(const InputData& data);

  ASGE::Point2D mouse_pos       = ASGE::Point2D(0, 0);
  std::atomic<bool> mouse_click = false;
  std::atomic<bool> key_pressed = false;
  std::atomic<int> key_value    = 0;

  int key_callback_id   = -1; /**< Key Input Callback ID. */
  int move_callback_id  = -1; /**< Key Input Callback ID. */
  int click_callback_id = -1; /**< Key Input Callback ID. */

  std::atomic<bool> is_active = true;
  std::mutex mutex_queue;
  std::mutex mutex_tile_clicked;
  std::queue<InputData> input_queue;
  ASGE::Input* asge_input = nullptr;
  std::vector<TileData> map;
  TileData* tile_clicked = nullptr;
};

#endif  // MYNETGAME_INPUT_H
