//
// Created by huxy on 23/02/2020.
//

#ifndef NETGAME_ASGEGAME_HPP
#define NETGAME_ASGEGAME_HPP

#include <Engine/OGLGame.h>
#include <gamelib/GCNetClient.hpp>
#include <gamelib/GCNetServer.hpp>
#include <gamelib/GameComponent.hpp>
#include <gamelib/ui/SceneManager.h>
#include <gamelib/ui/ui_elements/Button.h>
#include <vector>

#include "../include/gamelib/Map.h"

class Game : public ASGE::OGLGame
{
 public:
  explicit Game(const ASGE::GameSettings& settings);
  ~Game() override;

  Game(const Game&) = delete;
  Game& operator=(const Game&) = delete;

  // TEMPORARY INPUT STUFF WILL MOVE TO OWN THREAD / CLASS
  void keyHandler(ASGE::SharedEventData data);
  void moveHandler(ASGE::SharedEventData data);
  void clickHandler(ASGE::SharedEventData data);

  void update(const ASGE::GameTime& us) override;
  void render() override;

 private:
  bool loadFont();
  int font_index = 0;

  std::vector<std::unique_ptr<GameComponent>> game_components = {};
  SceneManager scene_manager;

  // TEMPORARY INPUT STUFF WILL MOVE TO OWN THREAD / CLASS
  ASGE::Point2D mouse_pos = ASGE::Point2D(0, 0);
  bool mouse_click        = false;

  int key_callback_id   = -1; /**< Key Input Callback ID. */
  int move_callback_id  = -1; /**< Key Input Callback ID. */
  int click_callback_id = -1; /**< Key Input Callback ID. */

  Map map;
};

#endif // NETGAME_ASGEGAME_HPP
