#include "ASGEGame.hpp"

#include <Engine/FileIO.h>
#include <Engine/Logger.hpp>

/// Initialises the game.
/// Setup your game and initialise the core components.
/// @param settings
Game::Game(const ASGE::GameSettings& settings) : OGLGame(settings)
{
  game_components.emplace_back(std::make_unique<GCNetServer>());
  game_components.emplace_back(std::make_unique<GCNetClient>());

  ASGE::Point2D cam_pivot;
  cam_pivot.x = 0;
  cam_pivot.y = 0;
  cam         = new ASGE::Camera2D(cam_pivot, 1280, 720);

  if (!loadFont())
  {
    Logging::log("\n*** FONT NOT LOADED ***\n");
  }

  inputs->use_threads = true;
  for (auto& gc : game_components)
  {
    if (!gc->init(renderer.get(), font_index))
    {
      Logging::log("\n*** COMPONENT NOT LOADED ***\n");
    }
    gc->addInputReader(*inputs);
  }

  Logging::log("\n--- GAME INITIALISED ---\n");
  toggleFPS();
}

/// Destroys the game.
// Game::~Game()
//{
// this->inputs->unregisterCallback(static_cast<unsigned int>(key_callback_id));
//}

/// Processes key inputs.
/// This function is added as a callback to handle the game's
/// keyboard input. For this game, calls to this function
/// are not thread safe, so you may alter the game's state
/// but your code needs to designed to prevent data-races.
/// @param data
/// @see KeyEvent

/// Updates your game and all it's components.
/// @param us
void Game::update(const ASGE::GameTime& us)
{
  for (auto& gc : game_components)
  {
    if (gc->update(us))
    {
      signalExit();
    }
  }
  // cam->translateX(500);
  // cam->update(us);

  // key_pressed = false;
}

/// Render your game and its scenes here.
void Game::render()
{
  renderer->setFont(font_index);

  for (auto& gc : game_components) { gc->render(); }
}

bool Game::loadFont()
{
  using File = ASGE::FILEIO::File;
  File file  = File();
  if (file.open("data/font/kenvector_future.ttf", ASGE::FILEIO::File::IOMode::READ))
  {
    // the file is open, but we need to read the contents of it into memory
    // we will use an IOBuffer for this, it will store the binary data read
    using Buffer  = ASGE::FILEIO::IOBuffer;
    Buffer buffer = file.read();

    // if we have data, load the font
    if (buffer.length != 0)
    {
      font_index =
        renderer->loadFontFromMem("Kenney", buffer.as_unsigned_char(), buffer.length, 32);
      file.close();
      return true;
    }

    file.close();
    return false;
  }

  return false;
}