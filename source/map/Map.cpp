//
// Created by l4-neale on 13/03/2020.
//

#include "Map/Map.h"

#include <Engine/FileIO.h>
#include <Engine/Logger.hpp>
#include <iostream>
#include <nlohmann/json.hpp>
#include <random>

using nlohmann::json;

void Map::init(const int screen_width, const int screen_height)
{
  map.clear();
  readJSON("data/MapTileTypes - Copy.json");
  this->screen_width  = screen_width;
  this->screen_height = screen_height;
}

void Map::readJSON(const std::string& directory)
{
  grass.clear();
  sand.clear();
  mix.clear();
  using File = ASGE::FILEIO::File;
  File file  = File();

  if (file.open(directory))
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
        "data/sprites/map/" + json_file["grass"][i]["directory"].get<std::string>();
      // tile_data_grass.movement_speed = json_file["grass"][i]["movement_speed"].get<int>();
      grass.push_back(tile_data_grass);

      TileData tile_data_sand;
      tile_data_sand.name = json_file["sand"][i]["name"].get<std::string>();
      tile_data_sand.directory =
        "data/sprites/map/" + json_file["sand"][i]["directory"].get<std::string>();
      //  tile_data_sand.movement_speed = json_file["sand"][i]["movement_speed"].get<int>();
      sand.push_back(tile_data_sand);
    }
    for (int j = 0; j < std::stoi(mix_count); ++j)
    {
      TileData tile_data_mix;
      tile_data_mix.name = json_file["mix"][j]["name"].get<std::string>();
      tile_data_mix.directory =
        "data/sprites/map/" + json_file["mix"][j]["directory"].get<std::string>();
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
      current_tile.tile_id   = i + tiles_wide * j;
      current_tile.sprite    = renderer->createRawSprite();
      current_tile.sprite->loadTexture(current_tile.directory);
      current_tile.sprite->xPos(static_cast<float>(i * tile_width));
      current_tile.sprite->yPos(static_cast<float>(j * tile_height));
      current_tile.sprite->width(static_cast<float>(tile_width));
      current_tile.sprite->height(static_cast<float>(tile_height));
      current_tile.sprite->setGlobalZOrder(-10);
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

void Map::readLevelJson(const std::string& directory)
{
  using File = ASGE::FILEIO::File;
  File file  = File();

  if (file.open(directory))
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

bool Map::checkTileName(const std::vector<TileData>& tiles, const std::string& to_find)
{
  auto it = std::find_if(tiles.begin(), tiles.end(), [&to_find](const TileData& tile) {
    return tile.name == to_find;
  });

  if (it != tiles.end())
  {
    map.push_back(*it);
    return true;
  }

  return false;
}

std::vector<TileData>* Map::getMap()
{
  return &map;
}

TileData* Map::getTile(int id)
{
  return &map.at(id);
}

bool Map::tileInRange(int tile_id_one, int tile_id_two, int range) const
{
  int x_index_one = tile_id_one % tiles_wide;
  int x_index_two = tile_id_two % tiles_wide;
  int width_diff  = abs(x_index_one - x_index_two);

  int y_index_one = tile_id_one / tiles_wide;
  int y_index_two = tile_id_two / tiles_wide;
  int height_diff = abs(y_index_one - y_index_two);

  return width_diff + height_diff <= range;
}

void Map::addSpawnBase(int _player_id)
{
  bool left  = _player_id == 1;
  bool found = false;
  std::random_device gen;
  std::mt19937 mt(gen());
  std::uniform_int_distribution<int> distribution(0, tiles_wide * tiles_high);
  do
  {
    int num = distribution(mt);
    for (auto& tile : map)
    {
      if (left && tile.tile_id == num)
      {
        if ((int)tile.sprite->xPos() < base_tile_distance * tile_width)
        {
          tile.is_base = true;
          tile.sprite->colour(ASGE::COLOURS::BLUE);
          base_camp = &tile;
          found     = true;
        }
        break;
      }
      if (!left && tile.tile_id == num)
      {
        if ((int)tile.sprite->xPos() > (tiles_wide - base_tile_distance) * tile_width)
        {
          tile.is_base = true;
          tile.sprite->colour(ASGE::COLOURS::BLUE);
          base_camp = &tile;
          found     = true;
        }
        break;
      }
    }
  } while (!found);
}

TileData* Map::getBaseCamp()
{
  return base_camp;
}

/*
 * calculates whether you can place a troop there
 * needs to be within a certain amount of tiles to the base camp
 */
bool Map::inRangeOfBase(const TileData& _tile_data)
{
  std::array pos   = {_tile_data.sprite->xPos(), _tile_data.sprite->yPos()};
  float x_distance = base_camp->sprite->xPos() - pos[0];
  float y_distance = base_camp->sprite->yPos() - pos[1];

  float x_tiles = x_distance / (float)tile_width;
  float y_tiles = y_distance / (float)tile_width;
  return (abs(x_tiles) <= 3 && abs(y_tiles) <= 3);
}
