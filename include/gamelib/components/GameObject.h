//
// Created by a2-lott on 13/03/2020.
//

#ifndef MYNETGAME_GAMEOBJECT_H
#define MYNETGAME_GAMEOBJECT_H

#include "SpriteComponent.h"

class GameObject
{
 public:
  GameObject() = default;

  // Adds a sprite to game object.
  void addSpriteComponent(ASGE::Renderer* renderer, const std::string& texture_file_name);

  // Adds a sprite to game object and sets it's position.
  void addSpriteComponent(
    ASGE::Renderer* renderer,
    const std::string& texture_file_name,
    float x_pos,
    float y_pos);

  SpriteComponent* getSpriteComponent();

 private:
  SpriteComponent* sprite_component = nullptr;
};

#endif  // MYNETGAME_GAMEOBJECT_H
