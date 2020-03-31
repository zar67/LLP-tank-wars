//
// Created by huxy on 23/02/2020.
//

#ifndef NETGAME_GCNETCLIENT_HPP
#define NETGAME_GCNETCLIENT_HPP
#include "../InputManager.h"
#include "../Map/Map.h"
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
  bool update(double dt) override;
  bool updateUI();
  void render() override;

  void decodeMessage(const std::vector<char>& message);
  void encodeAction(NetworkMessages instruction, Types data);
  std::vector<std::string> getMessageData(std::vector<char> message);

  void endTurn();
  void startTurn();

  void startGame();

  void buyUnit(TroopTypes unit_type);

  void addInputReader(ASGE::Input& _inputs) override;

 private:
  ASGE::Renderer* renderer = nullptr;

  netlib::ClientConnection client;
  SceneManager scene_manager;

  std::atomic_bool exiting = false;
  std::vector<std::vector<char>> actions;
  bool can_start      = true;
  bool in_turn        = false;
  int current_turn_id = 1;

  int currency                           = 100;
  std::vector<std::vector<Troop>> troops = {{}, {}, {}, {}};
  Map map;
  InputManager* inputReader = nullptr;
  TileData* tile_clicked    = nullptr;
  int unit_count            = 0;
  // TODO: std::vector<Unit>() units;
};

#endif  // NETGAME_GCNETCLIENT_HPP
