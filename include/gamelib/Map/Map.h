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

  void init(int screen_width, int screen_height);
  void generateMap(ASGE::Renderer* renderer);
  void resetMap();
  void renderMap(ASGE::Renderer* renderer);

  std::vector<TileData>* getMap();
  TileData* getTile(int id);

  [[nodiscard]] bool tileInRange(int tile_id_one, int tile_id_two, int range) const;

  TileData* getBaseCamp(int player_index);
  bool inRangeOfBase(const TileData& _tile_data, int player_index);

 private:
  void readJSON(const std::string& directory);
  void readLevelJson(const std::string& directory);
  bool
  checkTileName(const std::vector<TileData>& tiles, const std::string& to_find, int player_base_id);

  int screen_width  = 0;
  int screen_height = 0;
  int tiles_wide    = 0;
  int tiles_high    = 0;
  int tile_width    = 0;
  int tile_height   = 0;

  std::vector<TileData> map{};
  std::vector<TileData> grass{};
  std::vector<TileData> sand{};
  std::vector<TileData> mix{};

  std::vector<TileData*> base_camps              = {nullptr, nullptr, nullptr, nullptr};
  const float SPAWN_RANGE                        = 3.0F;
};

#endif  // MYNETGAME_MAP_H
