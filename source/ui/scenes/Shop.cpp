//
// Created by z2-rowbotham on 11/03/2020.
//

#include "ui/scenes/Shop.h"

Shop::~Shop()
{
  for (Button* button : units) { delete button; }
  units.clear();

  for (ASGE::Sprite* sprite : unit_stats_ui) { delete sprite; }
  unit_stats_ui.clear();
}

Shop& Shop::operator=(const Shop& shop)
{
  if (&shop != this)
  {
    for (Button* button : units) { delete button; }
    units.clear();
    units = shop.units;

    for (ASGE::Sprite* sprite : unit_stats_ui) { delete sprite; }
    unit_stats_ui.clear();
    unit_stats_ui = shop.unit_stats_ui;
  }

  return *this;
}

bool Shop::init(ASGE::Renderer* renderer, int font_index, int player_id)
{
  shop_title = UIElement::setupText(
    renderer,
    font_index,
    "Shop",
    10 + (static_cast<float>(unit_types.size() * 70 - 10) / 2),
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
          10 + static_cast<float>(70 * count),
          90,
          50,
          50))
    {
      return false;
    }
    units.push_back(button);
    count += 1;
  }

  for (int i = 0; i < unit_types.size(); i++)
  {
    ASGE::Sprite* sprite = renderer->createRawSprite();
    if (!UIElement::setupSprite(
          *sprite, "data/sprites/ui/unit_stats.png", 10 + static_cast<float>(70 * i), 145, 30, 121))
    {
      return false;
    }
    unit_stats_ui.push_back(sprite);
  }

  count = 0;
  for (const std::string& cost : unit_data)
  {
    auto* text = new ASGE::Text(renderer->getFont(font_index), cost);
    text->setColour(ASGE::COLOURS::BLACK);
    text->setScale(0.7F);
    text->setPositionX(40 + static_cast<float>(70 * count));
    text->setPositionY(160);
    text->setZOrder(1);
    count += 1;
    cost_text.push_back(text);
  }

  return true;
}

UIElement::MenuItem
Shop::update(const ASGE::Point2D& cursor_pos, std::atomic<bool>& click, std::array<int, 2> cam_pos)
{
  int index = 0;
  for (Button* button : units)
  {
    button->update(cursor_pos, click, cam_pos);

    updateCameraX(cam_pos);
    updateCameraY(cam_pos);

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
  x_changed = false;
  y_changed = false;
  return UIElement::MenuItem::NONE;
}

void Shop::render(ASGE::Renderer* renderer)
{
  renderer->renderText(shop_title);

  for (Button* button : units) { button->render(renderer); }

  for (ASGE::Sprite* sprite : unit_stats_ui) { renderer->renderSprite(*sprite); }

  for (ASGE::Text* text : cost_text) { renderer->renderText(*text); }
}

void Shop::updateCameraX(std::array<int, 2> cam_pos)
{
  if (cam_pos[0] != local_cam_pos[0])
  {
    int x = cam_pos[0] - local_cam_pos[0];
    shop_title.setPositionX(shop_title.getPosition().x + static_cast<float>(x));
    if (!x_changed)
    {
      for (auto& text_comp : cost_text)
      { text_comp->setPositionX(text_comp->getPosition().x + static_cast<float>(x)); }

      for (auto* sprite : unit_stats_ui) { sprite->xPos(sprite->xPos() + static_cast<float>(x)); }
    }
    x_changed        = true;
    local_cam_pos[0] = cam_pos[0];
  }
}

void Shop::updateCameraY(std::array<int, 2> cam_pos)
{
  if (cam_pos[1] != local_cam_pos[1])
  {
    int y = cam_pos[1] - local_cam_pos[1];
    shop_title.setPositionY(shop_title.getPosition().y + static_cast<float>(y));
    if (!y_changed)
    {
      for (auto& text_comp : cost_text)
      { text_comp->setPositionY(text_comp->getPosition().y + static_cast<float>(y)); }

      for (auto* sprite : unit_stats_ui) { sprite->yPos(sprite->yPos() + static_cast<float>(y)); }
    }
    y_changed        = true;
    local_cam_pos[1] = cam_pos[1];
  }
}
