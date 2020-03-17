//
// Created by huxy on 23/02/2020.
//

#ifndef NETGAME_GCNETSERVER_HPP
#define NETGAME_GCNETSERVER_HPP

#include "GameComponent.hpp"
#include "MessageTypes.h"

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
  ~GCNetServer() final = default;

  std::string getIP();
  void update(double dt, SceneManager* scene_manager) override;

  GCNetServer(const GCNetServer&) = delete;
  GCNetServer& operator=(const GCNetServer&) = delete;

  void playerEndTurn();
  static void decodeMessage(const std::vector<char>& message);
  static std::vector<char> encodeMessage(ServerMessages message, int data);

 private:
  netlib::ServerConnection server;

  ServerState server_state = ServerState::NONE;
};

#endif  // NETGAME_GCNETSERVER_HPP
