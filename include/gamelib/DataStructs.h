//
// Created by l4-neale on 11/03/2020.
//

#ifndef MYNETGAME_DATASTRUCTS_H
#define MYNETGAME_DATASTRUCTS_H

struct Move
{
  int x_pos = 0;
  int y_pos = 0;
};

struct Attack
{
  int attacker_id = -1;
  int enenmy_id   = -1;
  int damage      = 0;
};

struct Buy
{
  int item_id = -1;
  int cost    = 0;
  Move pos;
};

struct Types
{
  Move move;
  Attack attack;
  Buy buy;
};

#endif // MYNETGAME_DATASTRUCTS_H
