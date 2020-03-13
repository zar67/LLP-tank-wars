#include "ASGEGame.hpp"
#include <Engine/FileIO.h>
#include <Engine/Logger.hpp>

/// Initialises the game.
/// Setup your game and initialise the core components.
/// @param settings
Game::Game(const ASGE::GameSettings& settings) : OGLGame(settings)
{
  key_callback_id =
    inputs->addCallbackFnc(ASGE::E_KEY, &Game::keyHandler, this);

  move_callback_id =
    inputs->addCallbackFnc(ASGE::E_MOUSE_MOVE, &Game::moveHandler, this);

  click_callback_id =
    inputs->addCallbackFnc(ASGE::E_MOUSE_CLICK, &Game::clickHandler, this);

  server = std::make_unique<GCNetServer>();
  client = std::make_unique<GCNetClient>();

  if (!loadFont())
  {
    Logging::log("*** FONT NOT LOADED ***\n");
  }

  if (!scene_manager.init(renderer.get(), font_index, settings.window_width))
  {
    Logging::log("*** SCENE MANAGER NOT LOADED ***\n");
  }

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
  server->update(us.deltaInSecs());
  client->update(us.deltaInSecs());

  UIElement::MenuItem item =
    scene_manager.update(mouse_pos, mouse_click, key_pressed, key_value);
  key_pressed = false;

  if (item == UIElement::MenuItem::EXIT_GAME)
  {
    signalExit();
  }
  else if (item == UIElement::MenuItem::HOST_GAME)
  {
    server->startServer();
    client->connectToIP("localHost");
  }
  else if (item == UIElement::MenuItem::OPEN_LOBBY)
  {
    std::string id = scene_manager.getJoinIP();
    client->connectToIP(id);
  }
  /*else if (item == UIElement::MenuItem::BUY_UNIT_0)
  {
    scene_manager.closeShop(); // IF ITEM BOUGHT, CLOSE THE SHOP AND PLACE UNIT
    std::cout << "BUY UNIT 0" << std::endl;
  }
  else if (item == UIElement::MenuItem::BUY_UNIT_1)
  {
    scene_manager.closeShop(); // IF ITEM BOUGHT, CLOSE THE SHOP AND PLACE UNIT
    std::cout << "BUY UNIT 1" << std::endl;
  }
  else if (item == UIElement::MenuItem::BUY_UNIT_2)
  {
    scene_manager.closeShop(); // IF ITEM BOUGHT, CLOSE THE SHOP AND PLACE UNIT
    std::cout << "BUY UNIT 2" << std::endl;
  }
  else if (item == UIElement::MenuItem::BUY_UNIT_3)
  {
    scene_manager.closeShop(); // IF ITEM BOUGHT, CLOSE THE SHOP AND PLACE UNIT
    std::cout << "BUY UNIT 3" << std::endl;
  }*/
}

/// Render your game and its scenes here.
void Game::render()
{
  renderer->setFont(font_index);
  scene_manager.render(renderer.get());
}

bool Game::loadFont()
{
  using File = ASGE::FILEIO::File;
  File file  = File();
  if (file.open("data/kenvector_future.ttf", ASGE::FILEIO::File::IOMode::READ))
  {
    // the file is open, but we need to read the contents of it into memory
    // we will use an IOBuffer for this, it will store the binary data read
    using Buffer  = ASGE::FILEIO::IOBuffer;
    Buffer buffer = file.read();

    Logging::log(std::to_string(buffer.length) + "\n");
    // if we have data, load the font
    if (buffer.length != 0)
    {
      font_index = renderer->loadFontFromMem(
        "Kenney", buffer.as_unsigned_char(), buffer.length, 32);
      file.close();
      return true;
    }

    file.close();
    return false;
  }

  return false;
}