//
// Created by l4-neale on 11/03/2020.
//

#ifndef MYNETGAME_DATASTRUCTS_H
#define MYNETGAME_DATASTRUCTS_H

#include "DataComp.h"

struct Move
{
  int unit_id = 0;
  int x_pos   = 0;
  int y_pos   = 0;
};

struct Attack
{
  int attacker_id = -1;
  int enemy_id    = -1;
  int damage      = 0;
};

struct Buy
{
  TroopTypes item_id = TroopTypes::TANK_RED;
  Move pos;
};

struct Types
{
  Move move;
  Attack attack;
  Buy buy;
};

#endif  // MYNETGAME_DATASTRUCTS_H
