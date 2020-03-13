//
// Created by z2-rowbotham on 11/03/2020.
//

#ifndef MYNETGAME_LOBBY_H
#define MYNETGAME_LOBBY_H

#include "../ui_elements/Button.h"
#include "../ui_elements/UIElement.h"
#include <Engine/Renderer.h>
#include <vector>

class Lobby
{
 public:
  Lobby() = default;
  ~Lobby();

  Lobby(const Lobby& lobby){};          // Copy Constructor
  Lobby& operator=(const Lobby& lobby); // Copy Assignment Operator

  bool init(ASGE::Renderer* renderer, int font_index);
  UIElement::MenuItem update(const ASGE::Point2D& cursor_pos, bool click);
  void render(ASGE::Renderer* renderer);

  bool addPlayer(ASGE::Renderer* renderer);

 private:
  ASGE::Text lobby_title                  = ASGE::Text();
  std::vector<ASGE::Sprite*> player_icons = {};
  Button start_game                       = Button();

  int player_icons_center = 0;
  int player_number       = 0;
};

#endif // MYNETGAME_LOBBY_H
