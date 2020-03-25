//
// Created by l4-neale on 11/03/2020.
//

#ifndef MYNETGAME_MESSAGETYPES_H
#define MYNETGAME_MESSAGETYPES_H

enum class NetworkMessages
{
  START_GAME         = 1,
  PLAYER_MOVE        = 2,
  PLAYER_ATTACK      = 3,
  PLAYER_BUY         = 4,
  PLAYER_NUM_CHANGED = 5,
  PLAYER_END_TURN    = 6,
  PLAYER_START_TURN  = 7
};

#endif  // MYNETGAME_MESSAGETYPES_H
