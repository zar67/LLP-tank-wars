//
// Created by a2-lott on 13/03/2020.
//

#include "gamedata/DataComp.h"

DataComp::TankDataStruct DataComp::getTankData(TroopTypes _type)
{
  return tankTypeToData[_type];
}
