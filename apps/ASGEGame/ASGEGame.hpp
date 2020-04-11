//
// Created by huxy on 23/02/2020.
//

#ifndef NETGAME_ASGEGAME_HPP
#define NETGAME_ASGEGAME_HPP

#include "gamelib/InputManager.h"

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

  void update(const ASGE::GameTime& us) override;
  void render() override;

 private:
  bool loadFont();
  int font_index = 0;

  std::vector<std::unique_ptr<GameComponent>> game_components;
  int key_callback_id   = -1; /**< Key InputManager Callback ID. */
  int move_callback_id  = -1; /**< Key InputManager Callback ID. */
  int click_callback_id = -1; /**< Key InputManager Callback ID. */
};

#endif  // NETGAME_ASGEGAME_HPP
