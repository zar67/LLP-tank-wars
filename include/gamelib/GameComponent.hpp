//
// Created by huxy on 23/02/2020.
//

#ifndef NETGAME_GAMECOMPONENT_HPP
#define NETGAME_GAMECOMPONENT_HPP

#include "ui/SceneManager.h"

#include <NetLib/NetworkEvent.h>
#include <queue>
#include <string>

class GameComponent
{
 public:
  enum class ID
  {
    INVALID_COMPONENT_ID = -1,
    NETWORK_CLIENT,
    NETWORK_SERVER,
    NUM_OF_SUPPORTED_COMPONENTS
  };

 public:
  explicit GameComponent(ID id) : id(id){};
  virtual ~GameComponent()                                    = default;
  virtual void update(double dt, SceneManager* scene_manager) = 0;

  GameComponent(const GameComponent&) = default;
  GameComponent(GameComponent&&)      = default;
  GameComponent& operator=(const GameComponent&) = default;
  GameComponent& operator=(GameComponent&&) = default;

 private:
  ID id = ID::INVALID_COMPONENT_ID;
};

#endif  // NETGAME_GAMECOMPONENT_HPP
