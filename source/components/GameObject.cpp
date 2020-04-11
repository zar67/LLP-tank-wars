//
// Created by a2-lott on 13/03/2020.
//

#include "components/GameObject.h"

void GameObject::addSpriteComponent(ASGE::Renderer* renderer, const std::string& texture_file_name)
{
  delete sprite_component;
  sprite_component = nullptr;
  sprite_component = new SpriteComponent();
  sprite_component->loadSprite(renderer, texture_file_name);
}

void GameObject::addSpriteComponent(
  ASGE::Renderer* renderer,
  const std::string& texture_file_name,
  float x_pos,
  float y_pos)
{
  delete sprite_component;
  sprite_component = nullptr;
  sprite_component = new SpriteComponent();
  sprite_component->loadSprite(renderer, texture_file_name);

  sprite_component->getSprite()->xPos(x_pos);
  sprite_component->getSprite()->yPos(y_pos);
}

void GameObject::addSpriteComponent(
  ASGE::Renderer* renderer,
  const std::string& texture_file_name,
  float x_pos,
  float y_pos,
  float width,
  float height)
{
  addSpriteComponent(renderer, texture_file_name, x_pos, y_pos);
  sprite_component->getSprite()->width(width);
  sprite_component->getSprite()->height(height);
}

SpriteComponent* GameObject::getSpriteComponent()
{
  if (sprite_component != nullptr)
  {
    return sprite_component;
  }

  return nullptr;
}
