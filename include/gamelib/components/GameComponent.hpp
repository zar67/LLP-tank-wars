//
// Created by huxy on 23/02/2020.
//

#ifndef NETGAME_GAMECOMPONENT_HPP
#define NETGAME_GAMECOMPONENT_HPP

#include "../gamedata/MessageTypes.h"
#include "../ui/SceneManager.h"

#include <NetLib/NetworkEvent.h>
#include <queue>
#include <string>

class GameComponent
{
 public:
  enum class ID
  {
    INVALID_COMPONENT_ID = -1,
    NETWORK_CLIENT       = 0,
    NETWORK_SERVER       = 1,
    NUM_OF_SUPPORTED_COMPONENTS
  };

 public:
  explicit GameComponent(ID id) : id(id){};
  virtual ~GameComponent() = default;

  virtual bool init(ASGE::Renderer* renderer, int font_index) { return true; };
  virtual bool update(ASGE::GameTime time) = 0;
  virtual void render(){};

  GameComponent(const GameComponent&) = default;
  GameComponent(GameComponent&&)      = default;
  GameComponent& operator=(const GameComponent&) = default;
  GameComponent& operator=(GameComponent&&) = default;

  virtual void addInputReader(ASGE::Input& _inputs) = 0;

 private:
  ID id         = ID::INVALID_COMPONENT_ID;
  int player_id = 0;
};

#endif  // NETGAME_GAMECOMPONENT_HPP
