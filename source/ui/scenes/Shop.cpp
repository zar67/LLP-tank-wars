//
// Created by z2-rowbotham on 11/03/2020.
//

#include "ui/scenes/Shop.h"

Shop::~Shop()
{
  for (Button* button : units)
  {
    delete button;
  }
  units.clear();
}

Shop& Shop::operator=(const Shop& shop)
{
  if (&shop != this)
  {
    for (Button* button : units)
    {
      delete button;
    }
    units.clear();
    units = shop.units;
  }

  return *this;
}

bool Shop::init(
  ASGE::Renderer* renderer, int font_index, int game_width,
  const std::vector<std::string>& unit_types)
{
  shop_title = UIElement::setupText(
    renderer,
    font_index,
    "Shop",
    20 + (static_cast<float>(unit_types.size() * 60 - 10) / 2),
    75,
    true,
    false,
    ASGE::COLOURS::BLACK,
    1);

  int count = 0;
  for (const std::string& texture : unit_types)
  {
    auto* button = new Button();
    if (!button->init(
          renderer,
          font_index,
          texture,
          texture,
          "",
          20 + static_cast<float>(60 * count),
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

UIElement::MenuItem Shop::update(const ASGE::Point2D& cursor_pos, bool click)
{
  if (!shop_active)
  {
    return UIElement::MenuItem::NONE;
  }

  int index = 0;
  for (Button* button : units)
  {
    button->update(cursor_pos, click);

    if (!button->pressed())
    {
      index += 1;
      continue;
    }

    switch (index)
    {
      case 0:
      {
        return UIElement::MenuItem::BUY_UNIT_0;
      }
      case 1:
      {
        return UIElement::MenuItem::BUY_UNIT_1;
      }
      case 2:
      {
        return UIElement::MenuItem::BUY_UNIT_2;
      }
      case 3:
      {
        return UIElement::MenuItem::BUY_UNIT_3;
      }
      default:
        break;
    }
  }

  return UIElement::MenuItem::NONE;
}

void Shop::render(ASGE::Renderer* renderer)
{
  if (shop_active)
  {
    renderer->renderText(shop_title);

    for (Button* button : units)
    {
      button->render(renderer);
    }
  }
}

void Shop::setActive(bool value)
{
  shop_active = value;
}

bool Shop::getActive()
{
  return shop_active;
}
