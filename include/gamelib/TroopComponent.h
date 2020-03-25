//
// Created by a2-lott on 13/03/2020.
//

#ifndef MYNETGAME_TROOPCOMPONENT_H
#define MYNETGAME_TROOPCOMPONENT_H

#include "GameObject.h"
#include "map"

#include "gamedata/DataComp.h"
#include <Engine/Logger.hpp>

class TroopComponent : GameObject
{
 public:
  // Default Troop component constructor.
  TroopComponent() = default;
  TroopComponent(const TroopComponent& troopComponent){}; // Copy Constructor
  TroopComponent& operator=(const TroopComponent& troopComponent); // Copy
                                                                   // Assignment
                                                                   // Operator
  // Constructor that creates a troop type and a sprite component.
  TroopComponent(TroopTypes type_to_make, ASGE::Renderer* renderer);

  // Constructor that creates a troop type and a sprite component and sets it's
  // position.
  TroopComponent(
    TroopTypes type_to_make, ASGE::Renderer* renderer, float x_pos,
    float y_pos);

  ~TroopComponent();

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

 private:
  DataComp* data                       = nullptr;
  DataComp::TankDataStruct troop_stats = { 0, 0, 0, 0, 0, "" };
  TroopTypes current_troop_type        = TroopTypes::TANK_BLUE;
};

#endif // MYNETGAME_TROOPCOMPONENT_H
