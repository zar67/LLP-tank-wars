//
// Created by a2-lott on 13/03/2020.
//

#include "Troop.h"

Troop::Troop(TroopTypes type_to_make, ASGE::Renderer* renderer, int player_id, bool owned)
{
  data        = new TroopData();
  troop_stats = data->getTankData(type_to_make);
  setTroopType(type_to_make);

  std::string texture = "data/sprites/troops/player_" + std::to_string(player_id) + "/";
  if (owned)
  {
    texture += "owned_";
  }
  texture += data->getTankData(current_troop_type).texture_path;

  addSpriteComponent(renderer, texture);
}

Troop::Troop(
  TroopTypes type_to_make,
  ASGE::Renderer* renderer,
  int x_pos,
  int y_pos,
  int player_id,
  bool owned)
{
  data        = new TroopData();
  troop_stats = data->getTankData(type_to_make);
  setTroopType(type_to_make);

  std::string texture = "data/sprites/troops/player_" + std::to_string(player_id) + "/";
  if (owned)
  {
    texture += "owned_";
  }
  texture += data->getTankData(current_troop_type).texture_path;

  addSpriteComponent(renderer, texture, static_cast<float>(x_pos), static_cast<float>(y_pos));

  // Center
  ASGE::Sprite* sprite = getSpriteComponent()->getSprite();
  sprite->xPos(sprite->xPos() - sprite->width() / 2);
  sprite->yPos(sprite->yPos() - sprite->height() / 2);
}

void Troop::setTroopType(TroopTypes new_type)
{
  Troop::current_troop_type = new_type;
}

int Troop::getHealth()
{
  return troop_stats.health;
}

int Troop::getAttackDamage()
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
