#include "ASGEGame.hpp"

#include <Engine/FileIO.h>
#include <Engine/Logger.hpp>

/// Initialises the game.
/// Setup your game and initialise the core components.
/// @param settings
Game::Game(const ASGE::GameSettings& settings) : OGLGame(settings)
{
  key_callback_id = inputs->addCallbackFnc(ASGE::E_KEY, &Game::keyHandler, this);

  move_callback_id = inputs->addCallbackFnc(ASGE::E_MOUSE_MOVE, &Game::moveHandler, this);

  click_callback_id = inputs->addCallbackFnc(ASGE::E_MOUSE_CLICK, &Game::clickHandler, this);

  game_components.emplace_back(std::make_unique<GCNetServer>());
  game_components.emplace_back(std::make_unique<GCNetClient>());

  if (!loadFont())
  {
    Logging::log("*** FONT NOT LOADED ***\n");
  }

  for (auto& gc : game_components)
  {
    if (!gc->init(renderer.get(), font_index))
    {
      Logging::log("*** COMPONENT NOT LOADED ***");
    }
  }

  inputs->use_threads = true;
  toggleFPS();

  // map.init(settings.window_width, settings.window_height);
  // map.generateMap(renderer.get());
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

  if (key->action == ASGE::KEYS::KEY_PRESSED)
  {
    key_pressed = true;
    key_value   = key->key;
  }
  else if (key->action == ASGE::KEYS::KEY_RELEASED)
  {
    key_pressed = false;
  }
}

void Game::moveHandler(ASGE::SharedEventData data)
{
  const auto* move = dynamic_cast<const ASGE::MoveEvent*>(data.get());

  mouse_pos.x = static_cast<float>(move->xpos);
  mouse_pos.y = static_cast<float>(move->ypos);
}

void Game::clickHandler(ASGE::SharedEventData data)
{
  const auto* click = dynamic_cast<const ASGE::ClickEvent*>(data.get());

  mouse_click = click->action == ASGE::MOUSE::BUTTON_PRESSED;
}

/// Updates your game and all it's components.
/// @param us
void Game::update(const ASGE::GameTime& us)
{
  for (auto& gc : game_components)
  {
    if (gc->update(us.deltaInSecs(), mouse_pos, mouse_click, key_pressed, key_value))
    {
      signalExit();
    }
  }

  key_pressed = false;
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