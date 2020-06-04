#include "ASGEGame.hpp"

#include <Engine/GameSettings.hpp>
#include <Engine/Logger.hpp>

int main(int argc, char* argv[])
{
  ASGE::GameSettings game_settings;
  game_settings.window_width  = 1280;
  game_settings.window_height = 720;
  game_settings.mode          = ASGE::GameSettings::WindowMode::WINDOWED;
  game_settings.anisotropic   = 16;
  game_settings.msaa_level    = 1;

  Logging::INFO("Launching Game!");

  bool is_server = false;
  if (argc > 1)
  {
    std::string{argv[1]} == "-server" ? is_server = true : is_server = false;
  }

  Game game(game_settings, is_server);
  game.run();

  return 0;
}