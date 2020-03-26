//
// Created by huxy on 23/02/2020.
//

#ifndef NETGAME_ASGEGAME_HPP
#define NETGAME_ASGEGAME_HPP

#include "../include/gamelib/Input.h"

#include <Engine/OGLGame.h>
#include <gamelib/components/GCNetClient.hpp>
#include <gamelib/components/GCNetServer.hpp>
#include <gamelib/components/GameComponent.hpp>
#include <gamelib/ui/SceneManager.h>
#include <gamelib/ui/ui_elements/Button.h>
#include <vector>

class Game : public ASGE::OGLGame
{
 public:
  explicit Game(const ASGE::GameSettings& settings);
  ~Game() override = default;

  Game(const Game&) = delete;
  Game& operator=(const Game&) = delete;

  // TEMPORARY INPUT STUFF WILL PLAYER_MOVE TO OWN THREAD / CLASS
  // void keyHandler(ASGE::SharedEventData data);
  // void moveHandler(ASGE::SharedEventData data);
  // void clickHandler(ASGE::SharedEventData data);

  void update(const ASGE::GameTime& us) override;
  void render() override;

 private:
  bool loadFont();
  int font_index = 0;

  Input* inputReader = nullptr;
  std::vector<std::unique_ptr<GameComponent>> game_components;

  // TEMPORARY INPUT STUFF WILL PLAYER_MOVE TO OWN THREAD / CLASS
  // ASGE::Point2D mouse_pos = ASGE::Point2D(0, 0);
  //  bool mouse_click        = false;
  // bool key_pressed        = false;
  // int key_value           = 0;

  int key_callback_id   = -1; /**< Key Input Callback ID. */
  int move_callback_id  = -1; /**< Key Input Callback ID. */
  int click_callback_id = -1; /**< Key Input Callback ID. */
};

#endif  // NETGAME_ASGEGAME_HPP
