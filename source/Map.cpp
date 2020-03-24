//
// Created by l4-neale on 13/03/2020.
//

#include "Map.h"

#include <Engine/FileIO.h>
#include <Engine/Logger.hpp>
#include <iostream>
#include <nlohmann/json.hpp>

using nlohmann::json;

void Map::init(ASGE::Renderer* renderer, const int screen_width, const int screen_height)
{
  readJSON("data/MapTileTypes.json", renderer);
  this->screen_width  = screen_width;
  this->screen_height = screen_height;
}

void Map::readJSON(const std::string& _directory, ASGE::Renderer* renderer)
{
  grass.clear();
  sand.clear();
  mix.clear();
  using File = ASGE::FILEIO::File;
  File file  = File();

  if (file.open(_directory))
  {
    using Buffer  = ASGE::FILEIO::IOBuffer;
    Buffer buffer = file.read();

    auto json_file = json::parse(buffer.as_char(), buffer.as_char() + buffer.length);

    file.close();

    auto terrain_count = json_file["terrain_count"].get<std::string>();
    auto mix_count     = json_file["mix_count"].get<std::string>();
    for (int i = 0; i < std::stoi(terrain_count); ++i)
    {
      TileData tile_data_grass;
      tile_data_grass.name = json_file["grass"][i]["name"].get<std::string>();
      ASGE::Sprite* g      = renderer->createRawSprite();
      std::string dir      = g->loadTexture(json_file["grass"][i]["directory"].get<std::string>());
      tile_data_grass.sprite = g;
      grass.push_back(tile_data_grass);

      TileData tile_data_sand;
      tile_data_sand.name = json_file["sand"][i]["name"].get<std::string>();
      ASGE::Sprite* s     = renderer->createRawSprite();
      s->loadTexture(json_file["sand"][i]["directory"].get<std::string>());
      tile_data_sand.sprite = s;
      sand.push_back(tile_data_sand);
    }
    for (int j = 0; j < std::stoi(mix_count); ++j)
    {
      TileData tile_data_mix;
      tile_data_mix.name = json_file["mix"][j]["name"].get<std::string>();
      ASGE::Sprite* m    = renderer->createRawSprite();
      m->loadTexture(json_file["mix"][j]["directory"].get<std::string>());
      tile_data_mix.sprite = m;
      mix.push_back(tile_data_mix);
    }
  }
}

void Map::generateMap()
{
  map.clear();
  tile_width  = screen_width / tiles_wide;
  tile_height = screen_height / tiles_high;
  TileData grass_tile;
  for (auto& tile : grass)
  {
    if (tile.name == "g")
    {
      grass_tile = tile;
      break;
    }
  }
  for (int i = 0; i < tiles_wide; ++i)
  {
    for (int j = 0; j < tiles_high; ++j) { map.push_back(*(new TileData(grass_tile))); }
  }
}

void Map::renderMap(ASGE::Renderer* renderer)
{
  if (map.empty())
  {
    return;
  }
  for (auto& tile : map) { renderer->renderSprite(*tile.sprite); }
}
