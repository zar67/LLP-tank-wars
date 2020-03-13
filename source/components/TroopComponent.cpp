//
// Created by a2-lott on 13/03/2020.
//

#include "TroopComponent.h"

TroopComponent::TroopComponent(TroopTypes typeToMake, ASGE::Renderer* renderer)
{
  data        = new DataComp();
  troop_stats = data->getTankData(typeToMake);
  setTroopType(typeToMake);
  addSpriteComponent(
    renderer, data->getTankData(current_troop_type).texture_path);
}

TroopComponent::TroopComponent(
  TroopTypes type_to_make, ASGE::Renderer* renderer, float x_pos, float y_pos)
{
  data        = new DataComp();
  troop_stats = data->getTankData(type_to_make);
  setTroopType(type_to_make);

  addSpriteComponent(
    renderer, data->getTankData(current_troop_type).texture_path, x_pos, y_pos);
}

void TroopComponent::setTroopType(TroopTypes new_type)
{
  TroopComponent::current_troop_type = new_type;
}

int TroopComponent::getHealth()
{
  return troop_stats.health;
}

int TroopComponent::getDamage()
{
  return troop_stats.damage;
}

int TroopComponent::getCost()
{
  return troop_stats.cost;
}

int TroopComponent::getMovementRange()
{
  return troop_stats.move_range;
}

int TroopComponent::getWeaponRange()
{
  return troop_stats.weapon_range;
}

void TroopComponent::takeDamage(int damageAmount)
{
  troop_stats.health -= damageAmount;
}

TroopTypes TroopComponent::getTroopType()
{
  return current_troop_type;
}

void TroopComponent::setTroopValues() {}

TroopComponent::~TroopComponent()
{
  delete data;
  data = nullptr;
}
