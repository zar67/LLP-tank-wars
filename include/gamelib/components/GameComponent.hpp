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
    NETWORK_CLIENT,
    NETWORK_SERVER,
    NUM_OF_SUPPORTED_COMPONENTS
  };

 public:
  explicit GameComponent(ID id) : id(id){};
  virtual ~GameComponent() = default;

  virtual bool init(ASGE::Renderer* renderer, int font_index) { return true; };
  virtual bool
  update(double dt, const ASGE::Point2D& cursor_pos, bool click, bool key_pressed, int key) = 0;
  virtual void render(const std::vector<TileData>& tile_data){};

  GameComponent(const GameComponent&) = default;
  GameComponent(GameComponent&&)      = default;
  GameComponent& operator=(const GameComponent&) = default;
  GameComponent& operator=(GameComponent&&) = default;

 private:
  ID id = ID::INVALID_COMPONENT_ID;
};

#endif  // NETGAME_GAMECOMPONENT_HPP
