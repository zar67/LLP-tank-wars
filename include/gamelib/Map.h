//
// Created by l4-neale on 13/03/2020.
//

#ifndef MYNETGAME_MAP_H
#define MYNETGAME_MAP_H
#include "TileData.h"

#include <Engine/Renderer.h>
#include <string>
#include <vector>

class Map
{
 public:
  Map()  = default;
  ~Map() = default;

  void init(ASGE::Renderer* renderer, int screen_width, int screen_height);
  void generateMap(ASGE::Renderer* renderer);

  void renderMap(ASGE::Renderer* renderer);

 private:
  void readJSON(const std::string& _directory, ASGE::Renderer* renderer);

  int screen_width  = 0;
  int screen_height = 0;
  int tiles_wide    = 40;
  int tiles_high    = 20;
  int tile_width    = 0;
  int tile_height   = 0;

  std::vector<TileData> map;
  std::vector<TileData> grass;
  std::vector<TileData> sand;
  std::vector<TileData> mix;
};

#endif  // MYNETGAME_MAP_H
