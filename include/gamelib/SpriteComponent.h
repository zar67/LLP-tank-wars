//
// Created by a2-lott on 13/03/2020.
//

#ifndef MYNETGAME_SPRITECOMPONENT_H
#define MYNETGAME_SPRITECOMPONENT_H

#include <Engine/Sprite.h>

class SpriteComponent
{
 public:
  SpriteComponent() = default;
  ~SpriteComponent();

  SpriteComponent(const SpriteComponent& spriteComponent){};
  SpriteComponent& operator=(const SpriteComponent& spriteComponent);
  bool
  loadSprite(ASGE::Renderer* renderer, const std::string& texture_file_name);

  ASGE::Sprite* getSprite();

 private:
  void free();
  ASGE::Sprite* sprite = nullptr;
};

#endif // MYNETGAME_SPRITECOMPONENT_H
