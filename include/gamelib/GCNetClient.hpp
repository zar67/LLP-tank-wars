//
// Created by huxy on 23/02/2020.
//

#ifndef NETGAME_GCNETCLIENT_HPP
#define NETGAME_GCNETCLIENT_HPP
#include "ActionStructs.h"
#include "GameComponent.hpp"
#include "MessageTypes.h"

#include <NetLib/ClientConnection.h>

class GCNetClient : public GameComponent
{
 public:
  GCNetClient();
  ~GCNetClient() override;
  GCNetClient(const GCNetClient&) = delete;
  GCNetClient& operator=(const GCNetClient&) = delete;

  bool init(ASGE::Renderer* renderer, int font_index) override;
  bool
  update(double dt, const ASGE::Point2D& cursor_pos, bool click, bool key_pressed, int key) override;
  bool updateUI(const ASGE::Point2D& cursor_pos, bool click, bool key_pressed, int key);
  void render(ASGE::Renderer* renderer) override;

  void decodeMessage(const std::vector<char>& message);
  void encodeAction(NetworkMessages instruction, ActionTypes data);

  void input();
  void endTurn();
  void startTurn();

  bool canStartGame();
  void startGame();

  void buyUnit(int unit_id);

 private:
  netlib::ClientConnection client;
  SceneManager scene_manager;

  std::atomic_bool exiting = false;
  std::vector<std::vector<char>> actions;
  bool can_start = false;
  bool in_turn   = false;

  int currency = 100;
  // TODO: std::vector<Unit>() units;
};

#endif  // NETGAME_GCNETCLIENT_HPP
