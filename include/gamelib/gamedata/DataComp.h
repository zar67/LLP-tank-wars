//
// Created by a2-lott on 13/03/2020.
//

#ifndef MYNETGAME_DATACOMP_H
#define MYNETGAME_DATACOMP_H

#include <map>
#include <string>
enum class TroopTypes
{
  NONE,
  TANK_BLUE,
  TANK_DARK,
  TANK_GREEN,
  TANK_RED,
  TANK_SAND
};

class DataComp
{
 public:
  struct TankDataStruct
  {
    int health               = 0;
    int damage               = 0;
    int cost                 = 0;
    int move_range           = 0;
    int weapon_range         = 0;
    std::string texture_path = "";

    // suggest turning this into an init/startup function so you can pool the
    // data struct - ask liam if unsure what i mean
    TankDataStruct(int _h, int _d, int _c, int _m, int _w, std::string _t) :
      texture_path(std::move(_t))
    {
      health       = _h;
      damage       = _d;
      cost         = _c;
      move_range   = _m;
      weapon_range = _w;
    }
    // somewhere you were trying to create a datastruct of this type using no
    // parameters
    TankDataStruct() = default;
  };

  TankDataStruct getTankData(TroopTypes _type);

 private:
  std::map<TroopTypes, TankDataStruct> tankTypeToData = {
    {TroopTypes::TANK_BLUE, TankDataStruct(0, 0, 0, 0, 0, "data/sprites/troops/tank_blue.png")},
    {TroopTypes::TANK_DARK, TankDataStruct(0, 0, 0, 0, 0, "data/sprites/troops/tank_dark.png")},
    {TroopTypes::TANK_GREEN, TankDataStruct(0, 0, 0, 0, 0, "data/sprites/troops/tank_green.png")},
    {TroopTypes::TANK_RED, TankDataStruct(0, 0, 0, 0, 0, "data/sprites/troops/tank_red.png")},
    {TroopTypes::TANK_SAND, TankDataStruct(0, 0, 0, 0, 0, "data/sprites/troops/tank_sand.png")}

  };
};

#endif  // MYNETGAME_DATACOMP_H
