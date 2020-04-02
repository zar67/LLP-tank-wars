//
// Created by l4-neale on 11/03/2020.
//

#ifndef MYNETGAME_DATASTRUCTS_H
#define MYNETGAME_DATASTRUCTS_H

#include "DataComp.h"

struct Move
{
  int unit_index      = -1;
  int current_tile_id = -1;
  int new_tile_id     = -1;
};

struct Attack
{
  int attacker_id = -1;
  int enemy_id    = -1;
  int damage      = -1;
};

struct Buy
{
  TroopTypes unit_type = TroopTypes::TANK_RED;
  int unit_id          = -1;
  int tile_id          = -1;
};

struct Types
{
  Move move;
  Attack attack;
  Buy buy;
};

#endif  // MYNETGAME_DATASTRUCTS_H
