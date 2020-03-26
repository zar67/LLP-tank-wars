//
// Created by huxy on 23/02/2020.
//

#ifndef NETGAME_GCNETCLIENT_HPP
#define NETGAME_GCNETCLIENT_HPP
#include "../Troop.h"
#include "../gamedata/DataStructs.h"
#include "../gamedata/MessageTypes.h"
#include "../map/Map.h"
#include "GameComponent.hpp"

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
  void render() override;

  void decodeMessage(const std::vector<char>& message);
  void encodeAction(NetworkMessages instruction, Types data);
  std::vector<std::string> getMessageData(std::vector<char> message);

  void endTurn();
  void startTurn();

  void startGame();

  void buyUnit(TroopTypes unit_type);

 private:
  ASGE::Renderer* renderer = nullptr;

  netlib::ClientConnection client;
  SceneManager scene_manager;

  std::atomic_bool exiting = false;
  std::vector<std::vector<char>> actions;
  bool can_start = false;
  bool in_turn   = false;

  int currency                           = 100;
  std::vector<std::vector<Troop>> troops = {{}, {}, {}, {}};
  Map map;
};

#endif  // NETGAME_GCNETCLIENT_HPP
