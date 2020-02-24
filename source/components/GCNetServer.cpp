//
// Created by huxy on 23/02/2020.
//

#include "GCNetServer.hpp"
GCNetServer::GCNetServer() : GameComponent(ID::NETWORK_SERVER)
{
  server.Start(32488);
}

void GCNetServer::update(double dt) {}
