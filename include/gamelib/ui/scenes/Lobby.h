//
// Created by z2-rowbotham on 11/03/2020.
//

#ifndef MYNETGAME_LOBBY_H
#define MYNETGAME_LOBBY_H

#include "../ui_elements/Button.h"
#include "../ui_elements/UIElement.h"

#include <Engine/Renderer.h>
#include <vector>
#include "../../Audio/AudioManager.h"

class Lobby
{
 public:
  Lobby() = default;
  ~Lobby();

  Lobby(const Lobby& lobby){};           // Copy Constructor
  Lobby& operator=(const Lobby& lobby);  // Copy Assignment Operator

  bool init(ASGE::Renderer* renderer, int font_index);
  UIElement::MenuItem update(AudioManager* audio_manager, const ASGE::Point2D& cursor_pos, bool click);
  void render(ASGE::Renderer* renderer);

  void setPlayerNumber(int number);

 private:
  void addPlayer(ASGE::Renderer* renderer);

  ASGE::Text lobby_title                  = ASGE::Text();
  std::vector<ASGE::Sprite*> player_icons = {};
  Button start_game                       = Button();
  Button back;

  int player_number = 0;
};

#endif  // MYNETGAME_LOBBY_H
