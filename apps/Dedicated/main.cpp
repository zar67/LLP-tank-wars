#include "GameServer.hpp"

#include <Engine/Logger.hpp>
#include <cstdio>

int main(int argc, char* argv[])
{
  Logging::INFO("Launching Dedicated Server!");
  DedicatedServer dedicated;
  dedicated.run();

  return 0;
}