//
// Created by a2-lott on 13/03/2020.
//

#include "gamedata/TroopData.h"

TroopData::TankDataStruct TroopData::getTankData(TroopTypes _type)
{
  return tankTypeToData[_type];
}
