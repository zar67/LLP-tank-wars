//
// Created by a2-lott on 13/03/2020.
//

#include "Troop.h"

Troop::Troop(TroopTypes type_to_make, ASGE::Renderer* renderer)
{
  data        = new DataComp();
  troop_stats = data->getTankData(type_to_make);
  setTroopType(type_to_make);
  addSpriteComponent(renderer, data->getTankData(current_troop_type).texture_path);
}

Troop::Troop(TroopTypes type_to_make, ASGE::Renderer* renderer, int x_pos, int y_pos)
{
  data        = new DataComp();
  troop_stats = data->getTankData(type_to_make);
  setTroopType(type_to_make);

  addSpriteComponent(
    renderer,
    data->getTankData(current_troop_type).texture_path,
    static_cast<float>(x_pos - 20),
    static_cast<float>(y_pos - 20),
    40,
    40);
}

void Troop::setTroopType(TroopTypes new_type)
{
  Troop::current_troop_type = new_type;
}

int Troop::getHealth()
{
  return troop_stats.health;
}

int Troop::getDamage()
{
  return troop_stats.damage;
}

int Troop::getCost()
{
  return troop_stats.cost;
}

int Troop::getMovementRange()
{
  return troop_stats.move_range;
}

int Troop::getWeaponRange()
{
  return troop_stats.weapon_range;
}

void Troop::takeDamage(int damageAmount)
{
  troop_stats.health -= damageAmount;
}

TroopTypes Troop::getTroopType()
{
  return current_troop_type;
}

void Troop::setTroopValues() {}

Troop::~Troop()
{
  delete data;
  data = nullptr;
}

Troop& Troop::operator=(const Troop& troopComponent)
{
  if (&troopComponent != this)
  {
    // add pointers in here
    delete data;
    this->data = troopComponent.data;
  }
  return *this;
}

Troop::Troop(const Troop& troopComponent) : GameObject(troopComponent) {}
