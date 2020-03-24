//
// Created by l4-neale on 11/03/2020.
//

#ifndef MYNETGAME_ACTIONSTRUCTS_H
#define MYNETGAME_ACTIONSTRUCTS_H

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
  int item_id = -1;
  Move pos;
};

struct ActionTypes
{
  Move move;
  Attack attack;
  Buy buy;
};

#endif  // MYNETGAME_ACTIONSTRUCTS_H
