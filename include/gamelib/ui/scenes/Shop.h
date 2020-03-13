//
// Created by z2-rowbotham on 11/03/2020.
//

#ifndef MYNETGAME_SHOP_H
#define MYNETGAME_SHOP_H

#include "../ui_elements/Button.h"
#include "../ui_elements/UIElement.h"
#include <Engine/Renderer.h>
#include <vector>

class Shop
{
 public:
  Shop() = default;
  ~Shop();

  Shop(const Shop& shop){};          // Copy Constructor
  Shop& operator=(const Shop& shop); // Copy Assignment Operator

  bool init(
    ASGE::Renderer* renderer, int font_index,
    const std::vector<std::string>& unit_types);
  UIElement::MenuItem update(const ASGE::Point2D& cursor_pos, bool click);
  void render(ASGE::Renderer* renderer);

 private:
  ASGE::Text shop_title;
  std::vector<Button*> units = {};
};

#endif // MYNETGAME_SHOP_H
