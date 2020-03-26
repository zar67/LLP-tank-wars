//
// Created by User on 26/03/2020.
//

#ifndef MYNETGAME_INPUTDATA_H
#define MYNETGAME_INPUTDATA_H

#include <Engine/OGLGame.h>

struct InputData
{
  ASGE::SharedEventData sharedEventData = ASGE::SharedEventData();
  ASGE::EventType eventType             = ASGE::EventType();
};

#endif  // MYNETGAME_INPUTDATA_H
