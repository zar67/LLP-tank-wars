//
// Created by huxy on 23/02/2020.
//

#ifndef NETGAME_GCNETSERVER_HPP
#define NETGAME_GCNETSERVER_HPP

#include "GameComponent.hpp"
#include <NetLib/ServerConnection.h>
class GCNetServer : public GameComponent
{
 public:
  GCNetServer();
  ~GCNetServer() final = default;
  void update(double dt) override;

  GCNetServer(const GCNetServer&) = delete;
  GCNetServer& operator=(const GCNetServer&) = delete;

 private:
  netlib::ServerConnection server;
};

#endif // NETGAME_GCNETSERVER_HPP
