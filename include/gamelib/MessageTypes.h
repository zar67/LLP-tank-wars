//
// Created by l4-neale on 11/03/2020.
//

#ifndef MYNETGAME_MESSAGETYPES_H
#define MYNETGAME_MESSAGETYPES_H

enum class Instructions
{
  MOVE   = 1,
  ATTACK = 2,
  BUY    = 3
};

enum class ServerMessages
{
  PLAYER_CONNECTED    = 1,
  PLAYER_DISCONNECTED = 2
};
#endif  // MYNETGAME_MESSAGETYPES_H
