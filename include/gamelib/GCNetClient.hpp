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

  void update(double dt, SceneManager* scene_manager) override;
  void decodeMessage(SceneManager* scene_manager, const std::vector<char>& message);
  void encodeAction(NetworkMessages instruction, ActionTypes data);

  GCNetClient(const GCNetClient&) = delete;
  GCNetClient& operator=(const GCNetClient&) = delete;

  bool connectToIP(const std::string& ip);
  void input();
  void endTurn();
  void startTurn();

  bool canStartGame();
  void startGame();

 private:
  netlib::ClientConnection client;
  std::atomic_bool exiting = false;
  std::vector<std::vector<char>> actions;
  bool can_start = false;
  bool in_turn   = false;
};

#endif  // NETGAME_GCNETCLIENT_HPP
