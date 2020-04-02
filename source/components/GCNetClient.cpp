//
// Created by huxy on 23/02/2020.
//

#include "components/GCNetClient.hpp"

#include <Engine/Logger.hpp>
#include <thread>

GCNetClient::GCNetClient() : GameComponent(ID::NETWORK_CLIENT)
{
  // client.ConnectToIP("localhost", 32488);
  // client.ConnectToIP("164.11.76.100", 32488);
}

GCNetClient::~GCNetClient()
{
  if (inputReader != nullptr)
  {
    inputReader->exitInputThread();
    delete (inputReader);
    inputReader = nullptr;
  }
  client.Disconnect();
}

bool GCNetClient::init(ASGE::Renderer* renderer, int font_index)
{
  this->renderer = renderer;
  map.init(1280, 720);
  map.generateMap(renderer);
  return scene_manager.init(renderer, font_index);
}

bool GCNetClient::update(double dt)
{
  if (updateUI())
  {
    return true;
  }

  std::queue<netlib::NetworkEvent> all_events = client.GetNetworkEvents();
  while (!all_events.empty())
  {
    netlib::NetworkEvent& event = all_events.front();
    switch (event.eventType)
    {
    case netlib::NetworkEvent::EventType::ON_CONNECT:
    {
      Logging::log("Connected to the server!\n");
      break;
    }
    case netlib::NetworkEvent::EventType::ON_DISCONNECT:
    {
      Logging::log("Disconnected from server! Shutting down...\n");
      break;
    }
    case netlib::NetworkEvent::EventType::MESSAGE:
    {
      decodeMessage(event.data);
      break;
    }
    }
    all_events.pop();
  }

  exiting = !client.IsRunning();
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  return false;
}

bool GCNetClient::updateUI()
{
  UIElement::MenuItem item = scene_manager.update(inputReader);

  switch (item)
  {
  case (UIElement::MenuItem::EXIT_GAME):
  {
    return true;
  }
  case (UIElement::MenuItem::START_GAME):
  {
    if (can_start)
    {
      scene_manager.screenOpen(SceneManager::Screens::GAME);
      startGame();
    }
    break;
  }
  case (UIElement::MenuItem::HOST_GAME):
  {
    client.ConnectToIP("localHost", 32488);
    break;
  }
  case (UIElement::MenuItem::BACK_TO_MENU):
  {
    client.Disconnect();
    break;
  }
  case (UIElement::MenuItem::CONNECT_TO_IP):
  {
    if (client.ConnectToIP(scene_manager.joinScreen()->getIP(), 32488))
    {
      scene_manager.screenOpen(SceneManager::Screens::LOBBY);
    }
    else
    {
      scene_manager.joinScreen()->displayConnectionError();
    }
    break;
  }
  case (UIElement::MenuItem::END_TURN):
  {
    endTurn();
    break;
  }
  case (UIElement::MenuItem::BUY_UNIT_0):
  {
    inputReader->deselectTile();
    shop_unit_selected = TroopTypes::TANK_BLUE;
    break;
  }
  case (UIElement::MenuItem::BUY_UNIT_1):
  {
    inputReader->deselectTile();
    shop_unit_selected = TroopTypes::TANK_DARK;
    break;
  }
  case (UIElement::MenuItem::BUY_UNIT_2):
  {
    inputReader->deselectTile();
    shop_unit_selected = TroopTypes::TANK_GREEN;
    break;
  }
  case (UIElement::MenuItem::BUY_UNIT_3):
  {
    inputReader->deselectTile();
    shop_unit_selected = TroopTypes::TANK_RED;
    break;
  }
  case (UIElement::MenuItem::BUY_UNIT_4):
  {
    inputReader->deselectTile();
    shop_unit_selected = TroopTypes::TANK_SAND;
    break;
  }
  case (UIElement::MenuItem::MAP_CLICK):
  {
    inputReader->setClickedMap(
      map.getMap(),
      *inputReader->mouseClicked(),
      inputReader->mousePos().x,
      inputReader->mousePos().y);

    TileData* tile_clicked       = inputReader->tileClicked();
    TileData* previously_clicked = inputReader->previousTileClicked();

    if (tile_clicked != nullptr)
    {
      if (shop_unit_selected != TroopTypes::NONE)
      {
        buyUnit(tile_clicked, shop_unit_selected);
        shop_unit_selected = TroopTypes::NONE;
      }
      else if (
        previously_clicked != nullptr && previously_clicked->tile_id != tile_clicked->tile_id &&
        previously_clicked->troop_player_id == clientIndexNumber() &&
        previously_clicked->troop_id > 0 && tile_clicked->troop_id <= 0 &&
        map.tileInRange(previously_clicked->tile_id, tile_clicked->tile_id, 2))
      {
        moveUnit(tile_clicked, previously_clicked);
      }
    }

    inputReader->unlockPreviousTile();
    inputReader->unlockTile();

    break;
  }
  }

  return false;
}

void GCNetClient::render()
{
  scene_manager.render(renderer, current_turn_id, in_turn, troops, *map.getMap(), currency);
}

void GCNetClient::decodeMessage(const std::vector<char>& message)
{
  auto type = static_cast<NetworkMessages>(message[0] - '0');

  switch (type)
  {
  case (NetworkMessages::START_GAME):
  {
    scene_manager.screenOpen(SceneManager::Screens::GAME);
    break;
  }
  case (NetworkMessages::PLAYER_NUM_CHANGED):
  {
    int player_num = static_cast<int>(message[2] - '0');
    scene_manager.lobbyScreen()->setPlayerNumber(player_num);
    // can_start = player_num >= 2;
    break;
  }
  case (NetworkMessages::PLAYER_END_TURN):
  {
    endTurn();
    break;
  }
  case (NetworkMessages::PLAYER_START_TURN):
  {
    current_turn_id = static_cast<int>(message[2] - '0');
    if (current_turn_id == client.GetUID())
    {
      in_turn = true;
    }

    break;
  }
  // MESSAGE FORMAT: TYPE:DATA,DATA,DATA:SENDER_ID
  case NetworkMessages::PLAYER_MOVE:
  {
    std::vector<std::string> data = getMessageData(message);
    int unit_id                   = std::stoi(data[0]);
    int current_tile_id           = std::stoi(data[1]);
    int new_tile_id               = std::stoi(data[2]);
    int sender_id                 = std::stoi(data[3]);

    TileData* current_tile = map.getTile(current_tile_id);
    TileData* new_tile     = map.getTile(new_tile_id);

    ASGE::Sprite* sprite = troops[sender_id][unit_id].getSpriteComponent()->getSprite();
    sprite->xPos(new_tile->sprite->xPos() + new_tile->sprite->width() / 2 - sprite->width() / 2);
    sprite->yPos(new_tile->sprite->yPos() + new_tile->sprite->height() / 2 - sprite->height() / 2);

    new_tile->troop_player_id     = sender_id;
    new_tile->troop_id            = unit_id;
    current_tile->troop_player_id = -1;
    current_tile->troop_id        = -1;

    break;
  }
  case NetworkMessages::PLAYER_ATTACK:
  {
    std::vector<std::string> data = getMessageData(message);
    int attacker_id               = std::stoi(data[0]);
    int unit_id                   = std::stoi(data[1]);
    int damage                    = std::stoi(data[2]);
    int sender_id                 = std::stoi(data[3]);

    troops[sender_id][unit_id].takeDamage(damage);

    // TODO: Troop Death
    break;
  }
  case NetworkMessages::PLAYER_BUY:
  {
    std::vector<std::string> data = getMessageData(message);
    auto unit_to_buy              = static_cast<TroopTypes>(std::stoi(data[0]));
    int unit_id                   = std::stoi(data[1]);
    int tile_id                   = std::stoi(data[2]);
    int sender_id                 = std::stoi(data[3]);

    TileData* tile = map.getTile(tile_id);

    int x_pos = static_cast<int>(tile->sprite->xPos() + tile->sprite->width() / 2);
    int y_pos = static_cast<int>(tile->sprite->yPos() + tile->sprite->height() / 2);

    troops[sender_id].emplace_back(Troop(unit_to_buy, renderer, x_pos, y_pos, false));

    tile->troop_id        = unit_id;
    tile->troop_player_id = sender_id;

    break;
  }
  }
}

void GCNetClient::encodeAction(NetworkMessages instruction, Types data)
{
  std::string string_message = std::to_string(static_cast<int>(instruction));
  std::vector<char> message;
  switch (instruction)
  {
  case NetworkMessages::PLAYER_MOVE:
    string_message += ":" + std::to_string(data.move.unit_id) + "," +
                      std::to_string(data.move.current_tile_id) + "," +
                      std::to_string(data.move.new_tile_id);
    break;
  case NetworkMessages::PLAYER_ATTACK:
    string_message += ":" + std::to_string(data.attack.attacker_id) + "," +
                      std::to_string(data.attack.enemy_id) + "," +
                      std::to_string(data.attack.damage);
    break;
  case NetworkMessages::PLAYER_BUY:
    string_message += ":" + std::to_string(static_cast<int>(data.buy.unit_type)) + "," +
                      std::to_string(data.buy.unit_id) + "," + std::to_string(data.buy.tile_index);
    break;
  }
  std::copy(string_message.begin(), string_message.end(), std::back_inserter(message));
  actions.push_back(message);
}

std::vector<std::string> GCNetClient::getMessageData(std::vector<char> message)
{
  std::vector<std::string> data;

  std::string current;
  for (int i = 2; i < message.size(); i++)
  {
    if (message[i] == ',')
    {
      data.push_back(current);
      current = "";
    }
    else
    {
      current += message[i];
    }
  }
  data.push_back(current);

  return data;
}

void GCNetClient::endTurn()
{
  if (in_turn)
  {
    in_turn = false;
    for (const auto& action : actions) { client.SendMessageToServer(action); }
    actions.clear();

    std::string string_message = std::to_string(static_cast<int>(NetworkMessages::PLAYER_END_TURN));
    std::vector<char> message;
    std::copy(string_message.begin(), string_message.end(), std::back_inserter(message));

    client.SendMessageToServer(message);
  }
}

void GCNetClient::startTurn()
{
  in_turn = true;
}

void GCNetClient::startGame()
{
  std::string string_message = std::to_string(static_cast<int>(NetworkMessages::START_GAME));
  std::vector<char> message;
  std::copy(string_message.begin(), string_message.end(), std::back_inserter(message));

  client.SendMessageToServer(message);
}

void GCNetClient::buyUnit(TileData* tile_clicked, TroopTypes unit_type)
{
  if (tile_clicked->troop_id > 0)
  {
    return;
  }
  int x_pos = static_cast<int>(tile_clicked->sprite->xPos() + tile_clicked->sprite->width() / 2);
  int y_pos = static_cast<int>(tile_clicked->sprite->yPos() + tile_clicked->sprite->height() / 2);

  Troop new_troop = Troop(unit_type, renderer, x_pos, y_pos, true);
  new_troop.setID(++unit_count);
  if (in_turn && currency >= new_troop.getCost())
  {
    currency -= new_troop.getCost();
    scene_manager.gameScreen()->closeShop();
    troops[clientIndexNumber()].push_back(new_troop);

    tile_clicked->troop_id        = new_troop.getID();
    tile_clicked->troop_player_id = clientIndexNumber();

    Types type;
    type.buy.unit_type  = unit_type;
    type.buy.unit_id    = new_troop.getID();
    type.buy.tile_index = tile_clicked->tile_id - 1;
    encodeAction(NetworkMessages::PLAYER_BUY, type);
  }
  inputReader->deselectTile();
}

void GCNetClient::moveUnit(TileData* tile_clicked, TileData* previously_clicked)
{
  if (tile_clicked != nullptr)  // TODO: IF IN RANGE
  {
    ASGE::Sprite* sprite = troops[clientIndexNumber()][previously_clicked->troop_id - 1]
                             .getSpriteComponent()
                             ->getSprite();
    sprite->xPos(
      tile_clicked->sprite->xPos() + tile_clicked->sprite->width() / 2 - sprite->width() / 2);
    sprite->yPos(
      tile_clicked->sprite->yPos() + tile_clicked->sprite->height() / 2 - sprite->height() / 2);

    tile_clicked->troop_id              = previously_clicked->troop_id;
    tile_clicked->troop_player_id       = clientIndexNumber();
    previously_clicked->troop_player_id = -1;
    previously_clicked->troop_id        = -1;

    Types type;
    type.move.unit_id;
    type.move.current_tile_id;
    type.move.new_tile_id;
    encodeAction(NetworkMessages::PLAYER_MOVE, type);
  }
}

void GCNetClient::addInputReader(ASGE::Input& _inputs)
{
  if (inputReader != nullptr)
  {
    delete (inputReader);
  }
  inputReader = new InputManager(_inputs);
}

int GCNetClient::clientIndexNumber()
{
  return static_cast<int>(client.GetUID()) - 1;
}
