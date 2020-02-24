//
// Created by huxy on 24/02/2020.
//

#include "GameServer.hpp"
#include <Engine/Logger.hpp>
void DedicatedServer::run()
{
  while (true)
  {
    server.update(1.0000 / 60);
  }
}