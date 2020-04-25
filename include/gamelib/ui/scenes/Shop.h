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

  Shop(const Shop& shop){};           // Copy Constructor
  Shop& operator=(const Shop& shop);  // Copy Assignment Operator

  bool init(ASGE::Renderer* renderer, int font_index, int player_id);
  UIElement::MenuItem
  update(const ASGE::Point2D& cursor_pos, std::atomic<bool>& click, std::array<int, 2> cam_pos);
  void render(ASGE::Renderer* renderer);

 private:
  void checkTroop(const std::string& value);

  ASGE::Text shop_title;
  std::vector<Button*> units         = {};
  std::vector<ASGE::Text*> cost_text = {};

  const std::vector<std::string> unit_types{"tank.png",
                                            "tank_big.png",
                                            "tank_large.png",
                                            "tank_huge.png"};

  const std::vector<std::string> unit_costs{"5", "15", "30", "50"};
  std::array<int, 2> local_cam_pos = {0, 0};
  bool x_changed                   = false;
  bool y_changed                   = false;

  std::vector<ASGE::Sprite*> unit_stats_ui{};

  const std::vector<std::string> unit_data{"5\n15\n5\n3\n1",
                                           "15\n20\n10\n2\n2",
                                           "30\n30\n20\n2\n2",
                                           "50\n40\n30\n1\n3"};
};

#endif  // MYNETGAME_SHOP_H
