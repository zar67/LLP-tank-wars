#include "ASGEGame.hpp"

/// Initialises the game.
/// Setup your game and initialise the core components.
/// @param settings
Game::Game(const ASGE::GameSettings& settings) : OGLGame(settings)
{
  key_callback_id =
    inputs->addCallbackFnc(ASGE::E_KEY, &Game::keyHandler, this);

  game_components.emplace_back(std::make_unique<GCNetServer>());
  game_components.emplace_back(std::make_unique<GCNetClient>());

  inputs->use_threads = true;
  toggleFPS();
}

/// Destroys the game.
Game::~Game()
{
  this->inputs->unregisterCallback(static_cast<unsigned int>(key_callback_id));
}

/// Processes key inputs.
/// This function is added as a callback to handle the game's
/// keyboard input. For this game, calls to this function
/// are not thread safe, so you may alter the game's state
/// but your code needs to designed to prevent data-races.
/// @param data
/// @see KeyEvent
void Game::keyHandler(ASGE::SharedEventData data)
{
  const auto* key = dynamic_cast<const ASGE::KeyEvent*>(data.get());

  if (key->key == ASGE::KEYS::KEY_ESCAPE)
  {
    signalExit();
  }
}

/// Updates your game and all it's components.
/// @param us
void Game::update(const ASGE::GameTime& us)
{
  for (auto& gc : game_components)
  {
    gc->update(us.deltaInSecs());
  }
}

/// Render your game and its scenes here.
void Game::render() {}
