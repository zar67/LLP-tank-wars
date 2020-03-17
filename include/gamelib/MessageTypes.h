//
// Created by l4-neale on 11/03/2020.
//

#ifndef MYNETGAME_MESSAGETYPES_H
#define MYNETGAME_MESSAGETYPES_H

enum class PlayerActions
{
  MOVE   = 1,
  ATTACK = 2,
  BUY    = 3
};

enum class ServerMessages
{
  PLAYER_NUM_CHANGED = 1
};

#endif  // MYNETGAME_MESSAGETYPES_H
