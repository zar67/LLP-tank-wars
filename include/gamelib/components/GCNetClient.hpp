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
#include "GameComponent.hpp"

#include <Engine/Camera2D.hpp>
#include <NetLib/ClientConnection.h>

class GCNetClient : public GameComponent
{
 public:
  GCNetClient();
  ~GCNetClient() override;
  GCNetClient(const GCNetClient&) = delete;
  GCNetClient& operator=(const GCNetClient&) = delete;

  bool init(ASGE::Renderer* renderer, int font_index) override;
  bool update(ASGE::GameTime time) override;
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
  void initGame();

  ASGE::Camera2D* cam      = nullptr;  // ASGE::Camera2D(1280, 720);
  ASGE::Renderer* renderer = nullptr;
  int font_index           = 0;

  netlib::ClientConnection client;
  SceneManager scene_manager;

  bool can_start      = true;
  bool in_turn        = false;
  int current_turn_id = 1;

  int max_time_units   = 3;
  int time_units_spent = 0;
  std::vector<std::vector<char>> actions;

  std::vector<std::vector<Troop*>> troops = {{}, {}, {}, {}};
  int unit_count                          = 0;

  int currency                               = 100;
  TroopTypes shop_unit_selected              = TroopTypes::NONE;
  std::vector<Troop*> units_bought_this_turn = {};

  Map map;
  InputManager* inputReader = nullptr;

  int player_id = 0;
};

#endif  // NETGAME_GCNETCLIENT_HPP
