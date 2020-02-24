//
// Created by huxy on 24/02/2020.
//

#ifndef NETGAME_DEDICATEDGAMESERVER_HPP
#define NETGAME_DEDICATEDGAMESERVER_HPP

#include <gamelib/GCNetServer.hpp>
class DedicatedServer
{
 public:
  DedicatedServer()  = default;
  ~DedicatedServer() = default;
  void run();

 private:
  GCNetServer server;
};

#endif // NETGAME_DEDICATEDGAMESERVER_HPP
