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

  Troop* getTroop(int player_id, int troop_id);

  void buyUnit(TileData* tile_clicked, TroopTypes unit_type);
  void moveUnit(TileData* tile_clicked, TileData* previously_clicked);
  void attackUnit(TileData* tile_clicked, TileData* previously_clicked);

  void addInputReader(ASGE::Input& _inputs) override;

 private:
  int clientIndexNumber();

  ASGE::Renderer* renderer = nullptr;
  int font_index           = 0;

  netlib::ClientConnection client;
  SceneManager scene_manager;

  std::atomic_bool exiting = false;
  std::vector<std::vector<char>> actions;
  bool can_start      = true;
  bool in_turn        = false;
  int current_turn_id = 1;

  int currency                            = 100;
  std::vector<std::vector<Troop*>> troops = {{}, {}, {}, {}};
  int unit_count                          = 0;
  TroopTypes shop_unit_selected           = TroopTypes::NONE;

  Map map;

  InputManager* inputReader = nullptr;
};

#endif  // NETGAME_GCNETCLIENT_HPP
