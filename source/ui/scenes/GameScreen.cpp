//
// Created by z2-rowbotham on 11/03/2020.
//

#include "ui/scenes/GameScreen.h"

#include <Engine/Logger.hpp>

GameScreen::~GameScreen()
{
  delete selected_box;
  selected_box = nullptr;
}

GameScreen& GameScreen::operator=(const GameScreen& gameScreen)
{
  if (&gameScreen != this)
  {
    delete selected_box;
    this->selected_box = gameScreen.selected_box;
  }

  return *this;
}

bool GameScreen::init(ASGE::Renderer* renderer, int font_index)
{
  if (!open_shop.init(
        renderer,
        font_index,
        "data/sprites/ui/button.png",
        "data/sprites/ui/button_pressed.png",
        "Open Shop",
        10,
        10,
        260,
        30))
  {
    return false;
  }

  if (!end_turn.init(
        renderer,
        font_index,
        "data/sprites/ui/button.png",
        "data/sprites/ui/button_pressed.png",
        "End Turn",
        static_cast<float>(ASGE::SETTINGS.window_width) - 260,
        static_cast<float>(ASGE::SETTINGS.window_height) - 40,
        250,
        30))
  {
    return false;
  }

  selected_box = renderer->createRawSprite();
  if (!UIElement::setupSprite(*selected_box, "data/sprites/ui/text_box.png", 20, 500, 225, 130))
  {
    return false;
  }
  selected_box->setGlobalZOrder(-1);

  return true;
}

UIElement::MenuItem GameScreen::update(
  const ASGE::Point2D& cursor_pos,
  std::atomic<bool>& click,
  std::array<int, 2> camera_pos)
{
  open_shop.update(cursor_pos, click, camera_pos);

  float x_diff = static_cast<float>(camera_pos[0]) - local_cam_pos[0];
  float y_diff = static_cast<float>(camera_pos[1]) - local_cam_pos[1];

  local_cam_pos = camera_pos;

  selected_box->xPos(selected_box->xPos() + x_diff);
  selected_box->yPos(selected_box->yPos() + y_diff);

  if (open_shop.pressed())
  {
    click = false;
    if (shop_active)
    {
      closeShop();
    }
    else
    {
      openShop();
    }
    return UIElement::MenuItem::SHOP_BUTTON;
  }

  end_turn.update(cursor_pos, click, camera_pos);
  if (end_turn.pressed())
  {
    click = false;
    return UIElement::MenuItem::END_TURN;
  }

  UIElement::MenuItem item = UIElement::MenuItem::NONE;

  if (shop_active)
  {
    item = shop.update(cursor_pos, click, camera_pos);
  }

  if (click)
  {
    item = UIElement::MenuItem::MAP_CLICK;
  }

  return item;
}

void GameScreen::render(
  ASGE::Renderer* renderer,
  int action_number,
  int current_player_turn,
  bool in_turn,
  bool alive,
  Troop* troop_selected,
  const int& currency)
{
  if (alive)
  {
    renderer->renderText(
      "Currency: " + std::to_string(currency),
      310 + local_cam_pos[0],
      35 + local_cam_pos[1],
      ASGE::COLOURS::WHITE);

    open_shop.render(renderer);

    if (shop_active)
    {
      shop.render(renderer);
    }

    if (in_turn)
    {
      renderer->renderText(
        "ACTIONS: " + std::to_string(action_number) + "/3",
        15 + local_cam_pos[0],
        ASGE::SETTINGS.window_height - 45 + local_cam_pos[1],
        ASGE::COLOURS::WHITE);
      renderer->renderText(
        "YOUR TURN",
        15 + local_cam_pos[0],
        ASGE::SETTINGS.window_height - 15 + local_cam_pos[1],
        ASGE::COLOURS::WHITE);
      end_turn.render(renderer);
    }
    else
    {
      renderer->renderText(
        "PLAYER " + std::to_string(current_player_turn) + "'S TURN",
        15 + local_cam_pos[0],
        ASGE::SETTINGS.window_height - 15 + local_cam_pos[1],
        ASGE::COLOURS::WHITE);
    }
  }
  else
  {
    renderer->renderText(
      "You Are Dead!",
      ASGE::SETTINGS.window_width / 2 - 160 + local_cam_pos[0],
      100 + local_cam_pos[1],
      ASGE::COLOURS::WHITE);
  }

  if (troop_selected != nullptr)
  {
    renderer->renderSprite(*selected_box);
    renderer->renderText(
      "Troop " + std::to_string(troop_selected->getID() + 1),
      35 + local_cam_pos[0],
      535 + local_cam_pos[1],
      ASGE::COLOURS::BLACK);
    renderer->renderText(
      "HP: " + std::to_string(troop_selected->getHealth()),
      35 + local_cam_pos[0],
      575 + local_cam_pos[1],
      ASGE::COLOURS::BLACK);
    renderer->renderText(
      "Atk: " + std::to_string(troop_selected->getAttackDamage()),
      35 + local_cam_pos[0] + local_cam_pos[1],
      615,
      ASGE::COLOURS::BLACK);
  }
}

bool GameScreen::initShop(ASGE::Renderer* renderer, int font_index, int player_id)
{
  return shop.init(renderer, font_index, player_id);
}

void GameScreen::openShop()
{
  shop_active = true;
  shop_title  = "Close Shop";
  open_shop.changeText("Close Shop");
}

void GameScreen::closeShop()
{
  shop_active = false;
  shop_title  = "Open Shop";
  open_shop.changeText("Open Shop");
}