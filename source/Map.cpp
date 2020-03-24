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
      tile_data_grass.directory =
        "data/sprites/" + json_file["grass"][i]["directory"].get<std::string>();
      tile_data_grass.sprite = renderer->createRawSprite();
      tile_data_grass.sprite->loadTexture(_directory);
      grass.push_back(tile_data_grass);
      if (tile_data_grass.name == "g")
      {
        std::string s = "et";
      }
      TileData tile_data_sand;
      tile_data_sand.name = json_file["sand"][i]["name"].get<std::string>();
      ASGE::Sprite* s     = renderer->createRawSprite();
      s->loadTexture("data/sprites/" + json_file["sand"][i]["directory"].get<std::string>());
      tile_data_sand.sprite = s;
      sand.push_back(tile_data_sand);
    }
    for (int j = 0; j < std::stoi(mix_count); ++j)
    {
      TileData tile_data_mix;
      tile_data_mix.name = json_file["mix"][j]["name"].get<std::string>();
      ASGE::Sprite* m    = renderer->createRawSprite();
      m->loadTexture("data/sprites/" + json_file["mix"][j]["directory"].get<std::string>());
      tile_data_mix.sprite = m;
      mix.push_back(tile_data_mix);
    }
  }
}

void Map::generateMap(ASGE::Renderer* renderer)
{
  map.clear();
  tile_width  = screen_width / tiles_wide;
  tile_height = screen_height / tiles_high;
  TileData grass_tile;
  for (auto& tile : grass)
  {
    if (tile.name == "g_LUB")
    {
      grass_tile = tile;
      break;
    }
  }
  for (int i = 0; i < tiles_wide; ++i)
  {
    for (int j = 0; j < tiles_high; ++j)
    {
      TileData current_tile;
      current_tile.sprite = renderer->createRawSprite();
      current_tile.sprite->loadTexture(grass_tile.directory);
      current_tile.sprite->xPos(static_cast<float>(i * tile_width));
      current_tile.sprite->yPos(static_cast<float>(j * tile_height));
      current_tile.sprite->width(static_cast<float>(tile_width));
      current_tile.sprite->height(static_cast<float>(tile_height));

      map.push_back(current_tile);
    }
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
