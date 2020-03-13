//
// Created by huxy on 23/02/2020.
//

#ifndef NETGAME_GCNETCLIENT_HPP
#define NETGAME_GCNETCLIENT_HPP
#include "DataStates.h"
#include "DataStructs.h"
#include "GameComponent.hpp"

#include <NetLib/ClientConnection.h>

class GCNetClient : public GameComponent
{
 public:
  GCNetClient();
  ~GCNetClient() override;

  void connectToIP(const std::string& ip);
  void update(double dt) override;

  GCNetClient(const GCNetClient&) = delete;
  GCNetClient& operator=(const GCNetClient&) = delete;

  void input();
  void endTurn();
  void encodeData(Instructions _instruction, Types _data);

 private:
  netlib::ClientConnection client;
  std::atomic_bool exiting = false;
  std::vector<std::vector<char>> actions;
};

#endif // NETGAME_GCNETCLIENT_HPP
