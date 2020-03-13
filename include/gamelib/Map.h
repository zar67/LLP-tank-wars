//
// Created by l4-neale on 13/03/2020.
//

#ifndef MYNETGAME_MAP_H
#define MYNETGAME_MAP_H
#include "TileData.h"
#include <Engine/Renderer.h>
#include <string>

class Map
{
 public:
  Map()  = default;
  ~Map() = default;

  void init(ASGE::Renderer* renderer);

 private:
  void readJSON(const std::string& _directory, ASGE::Renderer* renderer);

  int tiles_wide  = 0;
  int tiles_high  = 0;
  int tile_width  = 0;
  int tile_height = 0;
};

#endif // MYNETGAME_MAP_H
