//
// Created by l4-neale on 13/03/2020.
//

#ifndef MYNETGAME_TILEDATA_H
#define MYNETGAME_TILEDATA_H

#include <Engine/Sprite.h>
#include <string>

struct TileData
{
  std::string name      = "";
  std::string directory = "";
  ASGE::Sprite* sprite  = nullptr;
  int movement_speed = 0;  // if road move faster, if you cant travel on it speed = 0, max speed = 2
  int troop_player_id = -1;
  int troop_id        = -1;
  int tile_id         = -1;

  [[nodiscard]] int mouseClicked(float _x, float _y) const
  {
    if (
      _x > sprite->xPos() && _x < sprite->xPos() + sprite->width() && _y > sprite->yPos() &&
      _y < sprite->yPos() + sprite->height())
    {
      return tile_id;
    }
    return -1;
  }
};

#endif  // MYNETGAME_TILEDATA_H
