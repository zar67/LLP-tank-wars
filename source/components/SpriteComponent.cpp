//
// Created by a2-lott on 13/03/2020.
//

#include "components/SpriteComponent.h"

#include <Engine/Renderer.h>

SpriteComponent::~SpriteComponent()
{
  free();
}

bool SpriteComponent::loadSprite(ASGE::Renderer* renderer, const std::string& texture_file_name)
{
  free();
  sprite = renderer->createRawSprite();
  if (sprite->loadTexture(texture_file_name))
  {
    return true;
  }

  free();
  return false;
}

void SpriteComponent::free()
{
  if (sprite != nullptr)
  {
    delete sprite;
    sprite = nullptr;
  }
}

ASGE::Sprite* SpriteComponent::getSprite()
{
  return sprite;
}

SpriteComponent& SpriteComponent::operator=(const SpriteComponent& spriteComponent)
{
  if (&spriteComponent != this)
  {
    delete sprite;
    this->sprite = spriteComponent.sprite;
  }

  return *this;
}
