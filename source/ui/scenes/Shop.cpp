//
// Created by z2-rowbotham on 11/03/2020.
//

#include "ui/scenes/Shop.h"

Shop::~Shop()
{
  for (Button* button : units) { delete button; }
  units.clear();
}

Shop& Shop::operator=(const Shop& shop)
{
  if (&shop != this)
  {
    for (Button* button : units) { delete button; }
    units.clear();
    units = shop.units;
  }

  return *this;
}

bool Shop::init(ASGE::Renderer* renderer, int font_index, int player_id)
{
  shop_title = UIElement::setupText(
    renderer,
    font_index,
    "Shop",
    10 + (static_cast<float>(unit_types.size() * 60 - 10) / 2),
    75,
    true,
    false,
    ASGE::COLOURS::BLACK,
    1);

  int count = 0;
  for (const std::string& texture : unit_types)
  {
    std::string directory =
      "data/sprites/troops/player_" + std::to_string(player_id) + "/" + texture;
    auto* button = new Button();
    if (!button->init(
          renderer,
          font_index,
          directory,
          directory,
          "",
          10 + static_cast<float>(60 * count),
          90,
          50,
          50))
    {
      return false;
    }
    units.push_back(button);
    count += 1;
  }

  return true;
}

UIElement::MenuItem Shop::update(const ASGE::Point2D& cursor_pos, std::atomic<bool>& click)
{
  int index = 0;
  for (Button* button : units)
  {
    button->update(cursor_pos, click);

    if (!button->pressed())
    {
      index += 1;
      continue;
    }

    click = false;
    switch (index)
    {
    case 0:
    {
      return UIElement::MenuItem::BUY_NORMAL_TANK;
    }
    case 1:
    {
      return UIElement::MenuItem::BUY_BIG_TANK;
    }
    case 2:
    {
      return UIElement::MenuItem::BUY_LARGE_TANK;
    }
    case 3:
    {
      return UIElement::MenuItem::BUY_HUGE_TANK;
    }
    default:
    {
      break;
    }
    }
  }

  return UIElement::MenuItem::NONE;
}

void Shop::render(ASGE::Renderer* renderer)
{
  renderer->renderText(shop_title);

  for (Button* button : units) { button->render(renderer); }
}
