//
// Created by l4-neale on 13/03/2020.
//

#include "Map/Map.h"

#include "Troop.h"

#include <Engine/FileIO.h>
#include <Engine/Logger.hpp>
#include <iostream>
#include <nlohmann/json.hpp>
#include <random>
#include <ui/ui_elements/UIElement.h>

using nlohmann::json;

Map::~Map()
{
  for (auto* sprite : base_camp_sprites)
  {
    delete sprite;
    sprite = nullptr;
  }
  base_camp_sprites.clear();

  for (auto* sprite : foggy)
  {
    delete sprite;
    sprite = nullptr;
  }
  foggy.clear();
}

Map& Map::operator=(const Map& map)
{
  if (&map != this)
  {
    for (int i = 0; i < base_camp_sprites.size(); i++)
    {
      delete base_camp_sprites[i];
      base_camp_sprites[i] = map.base_camp_sprites[i];
    }

    for (int i = 0; i < foggy.size(); i++)
    {
      delete foggy[i];
      foggy[i] = map.foggy[i];
    }
  }

  return *this;
}

void Map::init(const int screen_width, const int screen_height)
{
  map.clear();
  readJSON("data/MapTileTypes - Copy.json");
  this->map_width  = screen_width * 2;
  this->map_height = screen_height * 2;
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
      tile_data_grass.foggy_directory =
        "data/sprites/foggymap/" + json_file["grass"][i]["directory"].get<std::string>();
      // tile_data_grass.movement_speed = json_file["grass"][i]["movement_speed"].get<int>();
      grass.push_back(tile_data_grass);

      TileData tile_data_sand;
      tile_data_sand.name = json_file["sand"][i]["name"].get<std::string>();
      tile_data_sand.directory =
        "data/sprites/map/" + json_file["sand"][i]["directory"].get<std::string>();
      tile_data_sand.foggy_directory =
        "data/sprites/foggymap/" + json_file["sand"][i]["directory"].get<std::string>();
      //  tile_data_sand.movement_speed = json_file["sand"][i]["movement_speed"].get<int>();
      sand.push_back(tile_data_sand);
    }
    for (int j = 0; j < std::stoi(mix_count); ++j)
    {
      TileData tile_data_mix;
      tile_data_mix.name = json_file["mix"][j]["name"].get<std::string>();
      tile_data_mix.directory =
        "data/sprites/map/" + json_file["mix"][j]["directory"].get<std::string>();
      tile_data_mix.foggy_directory =
        "data/sprites/foggymap/" + json_file["mix"][j]["directory"].get<std::string>();
      //  tile_data_mix.movement_speed = json_file["mix"][j]["movement_speed"].get<int>();
      mix.push_back(tile_data_mix);
    }
  }
}

void Map::generateMap(ASGE::Renderer* renderer)
{
  readLevelJson("data/MAP_LAYOUT - Copy.json");
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

      if (current_tile.player_base_id != -1)
      {
        current_tile.visible = false;
        current_tile.is_base = true;
        current_tile.sprite->colour(ASGE::COLOURS::GREYBLACK);
        base_camps.at(current_tile.player_base_id) = &current_tile;

        ASGE::Sprite* sprite = renderer->createRawSprite();
        std::string texture  = "data/sprites/troops/base_camp_" +
                              std::to_string(current_tile.player_base_id + 1) + ".png";
        if (!UIElement::setupSprite(
              *sprite,
              texture,
              current_tile.sprite->xPos(),
              current_tile.sprite->yPos(),
              static_cast<float>(tile_width),
              static_cast<float>(tile_height)))
        {
          Logging::log("*** BASE CAMP SPRITE NOT LOADED ***");
        }

        sprite->setGlobalZOrder(-6);
        base_camp_sprites[current_tile.player_base_id] = sprite;
      }
    }

    tile_ids_in_range = {-3 * map_width,
                         -2 * map_width - 1,
                         -2 * map_width,
                         -2 * map_width + 1,
                         -map_width - 2,
                         -map_width - 1,
                         -map_width,
                         -map_width + 1,
                         -map_width + 2,
                         -3,
                         -2,
                         -1,
                         0,
                         1,
                         2,
                         3,
                         map_width - 2,
                         map_width - 1,
                         map_width,
                         map_width + 1,
                         map_width + 2,
                         2 * map_width - 1,
                         2 * map_width,
                         2 * map_width + 1,
                         3 * map_width};
  }

  for (TileData& tile : map)
  {
    ASGE::Sprite* foggy_tile = renderer->createRawSprite();

    if (!foggy_tile->loadTexture(tile.foggy_directory))
    {
      Logging::log("*** FOGGY SPRITE " + tile.foggy_directory + " NOT LOADED ***");
    }

    foggy_tile->xPos(tile.sprite->xPos());
    foggy_tile->yPos(tile.sprite->yPos());
    foggy_tile->width(tile.sprite->width());
    foggy_tile->height(tile.sprite->height());
    foggy_tile->setGlobalZOrder(-4);
    foggy.push_back(foggy_tile);
  }
}

void Map::resetMap()
{
  for (auto tile : map)
  {
    tile.troop_player_id = -1;
    tile.troop_id        = -1;
  }
}

void Map::renderMap(ASGE::Renderer* renderer)
{
  if (map.empty())
  {
    return;
  }

  for (int i = 0; i < map.size(); i++)
  {
    if (map.at(i).visible)
    {
      renderer->renderSprite(*map.at(i).sprite);

      if (map.at(i).player_base_id != -1 && base_camp_sprites[map.at(i).player_base_id] != nullptr)
      {
        renderer->renderText(
          std::to_string(map.at(i).base_health),
          static_cast<int>(map.at(i).sprite->xPos()),
          static_cast<int>(map.at(i).sprite->yPos() + 20),
          ASGE::COLOURS::WHITE);
        renderer->renderSprite(*base_camp_sprites[map.at(i).player_base_id]);
      }
    }
    else
    {
      renderer->renderSprite(*foggy.at(i));
    }
  }
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
    tile_width     = map_width / tiles_wide;
    tile_height    = map_height / tiles_high;

    std::vector<std::string> tile_names;
    std::vector<int> player_bases;

    for (int i = 0; i < tiles_wide * tiles_high; ++i)
    {
      if (tile_names.max_size() >= i)
      {
        tile_names.push_back(json_file["TILES"][i]["name"].get<std::string>());
        player_bases.push_back(json_file["TILES"][i]["player_base_camp"].get<int>());
      }
    }

    for (int i = 0; i < tile_names.size(); i++)
    {
      if (
        checkTileName(grass, tile_names.at(i), player_bases.at(i)) ||
        checkTileName(sand, tile_names.at(i), player_bases.at(i)) ||
        checkTileName(mix, tile_names.at(i), player_bases.at(i)))
      {
        continue;
      }

      Logging::log("*** COULDN'T FIND TILE " + tile_names.at(i) + " ***");
    }

    file.close();
  }
}

bool Map::checkTileName(
  const std::vector<TileData>& tiles,
  const std::string& to_find,
  int player_base_id)
{
  auto it = std::find_if(tiles.begin(), tiles.end(), [&to_find](const TileData& tile) {
    return tile.name == to_find;
  });

  if (it != tiles.end())
  {
    map.push_back(*it);
    map.back().player_base_id = player_base_id;
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

TileData* Map::getBaseCamp(int player_index)
{
  return base_camps.at(player_index);
}

/*
 * calculates whether you can place a troop there
 * needs to be within a certain amount of tiles to the base camp
 */
bool Map::inRangeOfBase(const TileData& _tile_data, int player_index)
{
  std::array pos = {_tile_data.sprite->xPos(), _tile_data.sprite->yPos()};
  int x_distance =
    static_cast<int>(base_camps.at(player_index)->sprite->xPos()) - static_cast<int>(pos[0]);
  int y_distance =
    static_cast<int>(base_camps.at(player_index)->sprite->yPos()) - static_cast<int>(pos[1]);

  int x_tiles = x_distance / (tile_width);
  int y_tiles = y_distance / (tile_height);
  return static_cast<float>(abs(x_tiles)) <= SPAWN_RANGE &&
         static_cast<float>(abs(y_tiles)) <= SPAWN_RANGE;
}

void Map::setBaseCamps(int num_players)
{
  for (TileData& tile : map)
  {
    if (tile.player_base_id > -1 && tile.player_base_id >= num_players)
    {
      base_camp_sprites.at(tile.player_base_id) = nullptr;
      base_camps.at(tile.player_base_id)        = nullptr;
      tile.visible                              = false;
      tile.is_base                              = false;
      tile.player_base_id                       = -1;
      tile.sprite->colour(ASGE::COLOURS::WHITE);
    }
  }
}

void Map::updateVisibility(int player_id)
{
  std::vector<TileData*> troop_tiles;
  for (TileData& tile : map)
  {
    tile.visible = tileInRange(tile.tile_id, base_camps.at(player_id)->tile_id, 3);

    if (tile.troop_id == -1 || tile.troop_player_id != player_id)
    {
      continue;
    }

    setVisibleTilesInRange(tile.tile_id);
  }
}

void Map::setVisibleTilesInRange(int tile_id)
{
  for (int id_diff : tile_ids_in_range)
  {
    if (tile_id + id_diff > 0 && tile_id + id_diff < map.size())
    {
      map.at(tile_id + id_diff).visible = true;
    }
  }
}
