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
                           // if on road
  int troop_id = -1;
};

#endif  // MYNETGAME_TILEDATA_H
