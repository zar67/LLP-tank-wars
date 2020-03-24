//
// Created by l4-neale on 11/03/2020.
//

#ifndef MYNETGAME_MESSAGETYPES_H
#define MYNETGAME_MESSAGETYPES_H

enum class NetworkMessages
{
  PLAYER_MOVE        = 1,
  PLAYER_ATTACK      = 2,
  PLAYER_BUY         = 3,
  PLAYER_NUM_CHANGED = 4,
  PLAYER_END_TURN    = 5,
  PLAYER_START_TURN  = 6
};

#endif  // MYNETGAME_MESSAGETYPES_H
