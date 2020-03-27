//
// Created by l4-neale on 13/03/2020.
//

#include "map/Map.h"

#include <Engine/FileIO.h>
#include <Engine/Logger.hpp>
#include <iostream>
#include <nlohmann/json.hpp>

using nlohmann::json;

void Map::init(const int screen_width, const int screen_height)
{
  map.clear();
  readJSON("data/MapTileTypes.json");
  this->screen_width  = screen_width;
  this->screen_height = screen_height;
}

void Map::readJSON(const std::string& _directory)
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
      // tile_data_grass.movement_speed = json_file["grass"][i]["movement_speed"].get<int>();
      grass.push_back(tile_data_grass);

      TileData tile_data_sand;
      tile_data_sand.name = json_file["sand"][i]["name"].get<std::string>();
      tile_data_sand.directory =
        "data/sprites/" + json_file["sand"][i]["directory"].get<std::string>();
      //  tile_data_sand.movement_speed = json_file["sand"][i]["movement_speed"].get<int>();
      sand.push_back(tile_data_sand);
    }
    for (int j = 0; j < std::stoi(mix_count); ++j)
    {
      TileData tile_data_mix;
      tile_data_mix.name = json_file["mix"][j]["name"].get<std::string>();
      tile_data_mix.directory =
        "data/sprites/" + json_file["mix"][j]["directory"].get<std::string>();
      //  tile_data_mix.movement_speed = json_file["mix"][j]["movement_speed"].get<int>();
      mix.push_back(tile_data_mix);
    }
  }
}

void Map::generateMap(ASGE::Renderer* renderer)
{
  readLevelJson("data/MAP_LAYOUT.json");
  for (int i = 0; i < tiles_wide; ++i)
  {
    for (int j = 0; j < tiles_high; ++j)
    {
      TileData& current_tile = map.at(i + tiles_wide * j);
      current_tile.tile_id   = i + j + 1;
      current_tile.sprite    = renderer->createRawSprite();
      current_tile.sprite->loadTexture(current_tile.directory);
      current_tile.sprite->xPos(static_cast<float>(i * tile_width));
      current_tile.sprite->yPos(static_cast<float>(j * tile_height));
      current_tile.sprite->width(static_cast<float>(tile_width));
      current_tile.sprite->height(static_cast<float>(tile_height));
      current_tile.sprite->setGlobalZOrder(-1);
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

void Map::readLevelJson(const std::string& _directory)
{
  using File = ASGE::FILEIO::File;
  File file  = File();

  if (file.open(_directory))
  {
    using Buffer  = ASGE::FILEIO::IOBuffer;
    Buffer buffer = file.read();

    auto json_file = json::parse(buffer.as_char(), buffer.as_char() + buffer.length);
    tiles_wide     = json_file["number_tiles_wide"].get<int>();
    tiles_high     = json_file["number_tiles_high"].get<int>();
    tile_width     = screen_width / tiles_wide;
    tile_height    = screen_height / tiles_high;
    std::vector<std::string> tile_names;
    for (int i = 0; i < tiles_wide * tiles_high; ++i)
    {
      if (tile_names.max_size() >= i)
      {
        tile_names.push_back(json_file["TILES"][i]["name"].get<std::string>());
      }
    }
    for (auto& name : tile_names)
    {
      if (checkTileName(grass, name) || checkTileName(sand, name) || checkTileName(mix, name))
      {
        continue;
      }
    }
    file.close();
  }
}

bool Map::checkTileName(const std::vector<TileData>& _tiles, const std::string& _name)
{
  for (auto& tile : _tiles)
  {
    if (_name == tile.name)
    {
      map.push_back(tile);
      return true;
    }
  }
  return false;
}

std::vector<TileData> Map::getMap()
{
  return map;
}
