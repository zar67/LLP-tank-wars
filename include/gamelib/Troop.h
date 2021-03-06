//
// Created by a2-lott on 13/03/2020.
//

#ifndef MYNETGAME_TROOP_H
#define MYNETGAME_TROOP_H

#include "Map/Map.h"
#include "components/GameObject.h"
#include "gamedata/TroopData.h"

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
  Troop(int unit_id, TroopTypes type_to_make, ASGE::Renderer* renderer, int player_id, bool owned);

  // Constructor that creates a troop type and a sprite component and sets it's
  // position.
  Troop(
    int unit_id,
    TroopTypes type_to_make,
    ASGE::Renderer* renderer,
    int x_pos,
    int y_pos,
    int player_id,
    bool owned);

  // returns current troop type.
  TroopTypes getTroopType();

  // sets current troop type.
  void setTroopType(TroopTypes new_type);

  void setTroopValues();

  [[nodiscard]] int getHealth() const;
  [[nodiscard]] int getAttackDamage() const;
  [[nodiscard]] int getCost() const;
  [[nodiscard]] int getMovementRange() const;
  [[nodiscard]] int getWeaponRange() const;

  void takeDamage(int damageAmount);

  [[nodiscard]] int getID() const { return id; }

  [[nodiscard]] bool getBoughtThisTurn() const { return generated_this_turn; };
  void setBoughtThisTurn(bool value) { generated_this_turn = value; };

  [[nodiscard]] bool getAttackedThisTurn() const { return attacked_this_turn; };
  void setAttackedThisTurn(bool value) { generated_this_turn = value; };

 private:
  int id                                = -1;
  bool generated_this_turn              = false;
  bool attacked_this_turn               = false;
  TroopData* data                       = nullptr;
  TroopData::TankDataStruct troop_stats = {0, 0, 0, 0, 0, ""};
  TroopTypes current_troop_type         = TroopTypes::NONE;
};

#endif  // MYNETGAME_TROOP_H
