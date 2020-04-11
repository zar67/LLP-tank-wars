//
// Created by a2-lott on 13/03/2020.
//

#ifndef MYNETGAME_TROOPDATA_H
#define MYNETGAME_TROOPDATA_H

#include <map>
#include <string>
enum class TroopTypes
{
  NONE,
  NORMAL_TANK,
  BIG_TANK,
  LARGE_TANK,
  HUGE_TANK,
};

class TroopData
{
 public:
  struct TankDataStruct
  {
    int id                   = -1;
    int health               = 0;
    int damage               = 0;
    int cost                 = 0;
    int move_range           = 0;
    int weapon_range         = 0;
    std::string texture_path = "";

    // suggest turning this into an init/startup function so you can pool the
    // data struct - ask liam if unsure what i mean
    TankDataStruct(
      int tank_health,
      int attack_damage,
      int buy_cost,
      int move_distance,
      int attack_range,
      std::string texture) :
      texture_path(std::move(texture))
    {
      health       = tank_health;
      damage       = attack_damage;
      cost         = buy_cost;
      move_range   = move_distance;
      weapon_range = attack_range;
    }
    // somewhere you were trying to create a datastruct of this type using no
    // parameters
    TankDataStruct() = default;
  };

  TankDataStruct getTankData(TroopTypes _type);

 private:
  std::map<TroopTypes, TankDataStruct> tankTypeToData = {
    {TroopTypes::NORMAL_TANK, TankDataStruct(15, 5, 5, 3, 1, "tank.png")},
    {TroopTypes::BIG_TANK, TankDataStruct(20, 10, 15, 2, 2, "tank_big.png")},
    {TroopTypes::LARGE_TANK, TankDataStruct(30, 20, 30, 2, 2, "tank_large.png")},
    {TroopTypes::HUGE_TANK, TankDataStruct(40, 30, 50, 1, 3, "tank_huge.png")},

  };
};

#endif  // MYNETGAME_TROOPDATA_H
