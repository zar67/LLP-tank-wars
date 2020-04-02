//
// Created by a2-lott on 13/03/2020.
//

#ifndef MYNETGAME_TROOP_H
#define MYNETGAME_TROOP_H

#include "components/GameObject.h"
#include "gamedata/DataComp.h"
#include "map/Map.h"

#include <Engine/Renderer.h>

class Troop : public GameObject
{
 public:
  // Default Troop component constructor.
  Troop() = default;
  ~Troop();
  Troop(const Troop& troopComponent);             // Copy Constructor
  Troop& operator=(const Troop& troopComponent);  // Copy
                                                  // Assignment
                                                  // Operator
  // Constructor that creates a troop type and a sprite component.
  Troop(TroopTypes type_to_make, ASGE::Renderer* renderer, bool owned);

  // Constructor that creates a troop type and a sprite component and sets it's
  // position.
  Troop(TroopTypes type_to_make, ASGE::Renderer* renderer, int x_pos, int y_pos, bool owned);

  // returns current troop type.
  TroopTypes getTroopType();

  // sets current troop type.
  void setTroopType(TroopTypes new_type);

  void setTroopValues();

  int getHealth();

  int getDamage();

  int getCost();

  int getMovementRange();

  int getWeaponRange();

  void takeDamage(int damageAmount);

  int getID() { return id; }
  void setID(int _id) { id = _id; }

 private:
  int id                               = -1;  // probably move this to datacomp
  DataComp* data                       = nullptr;
  DataComp::TankDataStruct troop_stats = {0, 0, 0, 0, 0, ""};
  TroopTypes current_troop_type        = TroopTypes::TANK_BLUE;
};

#endif  // MYNETGAME_TROOP_H
