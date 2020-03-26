//
// Created by huxy on 23/02/2020.
//

#ifndef NETGAME_GCNETSERVER_HPP
#define NETGAME_GCNETSERVER_HPP

#include "../gamedata/MessageTypes.h"
#include "GameComponent.hpp"

#include <NetLib/ServerConnection.h>

class GCNetServer : public GameComponent
{
 public:
  enum class ServerState
  {
    NONE               = -1,
    START_GAME         = 0,
    WAITING_FOR_PLAYER = 1,
    UPDATING           = 2,
    NEXT_PLAYER_TURN   = 3
  };

  // include DataStates
  enum class MessageType
  {
    MOVE   = 1,
    ATTACK = 2,
    BUY    = 3
  };

 public:
  GCNetServer();
  ~GCNetServer() final            = default;
  GCNetServer(const GCNetServer&) = delete;
  GCNetServer& operator=(const GCNetServer&) = delete;
  bool init(ASGE::Renderer* renderer, int font_index) override;

  bool update(double dt) override;
  void decodeMessage(const std::vector<char>& message);
  std::vector<char> encodeMessage(NetworkMessages message, const std::string& data);

  std::string getIP();
  void playerEndTurn();
  void addInputReader(ASGE::Input& _inputs) override{};

 private:
  netlib::ServerConnection server;

  ServerState server_state = ServerState::NONE;
  int current_turn_id      = 1;
};

#endif  // NETGAME_GCNETSERVER_HPP
