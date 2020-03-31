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

  tile_clicked = inputReader->tileClicked();
  inputReader->unlockTile();
  // do stuff with tile then unlock it
  // inputReader->unlockTile();
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
    buyUnit(TroopTypes::TANK_BLUE);
    break;
  }
  case (UIElement::MenuItem::BUY_UNIT_1):
  {
    buyUnit(TroopTypes::TANK_DARK);
    break;
  }
  case (UIElement::MenuItem::BUY_UNIT_2):
  {
    buyUnit(TroopTypes::TANK_GREEN);
    break;
  }
  case (UIElement::MenuItem::BUY_UNIT_3):
  {
    buyUnit(TroopTypes::TANK_RED);
    break;
  }
  case (UIElement::MenuItem::BUY_UNIT_4):
  {
    buyUnit(TroopTypes::TANK_SAND);
    break;
  }
  case (UIElement::MenuItem::MAP_CLICK):
  {
    inputReader->setClickedMap(
      *inputReader->mouseClicked(), inputReader->mousePos().x, inputReader->mousePos().y);
    break;
  }
  }

  return false;
}

void GCNetClient::render()
{
  scene_manager.render(renderer, current_turn_id, in_turn, troops, map.getMap(), currency);
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
    float x_pos                   = std::stof(data[1]);
    float y_pos                   = std::stof(data[2]);
    int sender_id                 = std::stoi(data[3]);

    ASGE::Sprite* sprite = troops[sender_id][unit_id].getSpriteComponent()->getSprite();
    sprite->xPos(x_pos);
    sprite->yPos(y_pos);
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
    int x_pos                     = std::stoi(data[1]);
    int y_pos                     = std::stoi(data[2]);
    int sender_id                 = std::stoi(data[3]);

    troops[sender_id].emplace_back(Troop(unit_to_buy, renderer, x_pos, y_pos));
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
                      std::to_string(data.move.x_pos) + "," + std::to_string(data.move.y_pos);
    break;
  case NetworkMessages::PLAYER_ATTACK:
    string_message += ":" + std::to_string(data.attack.attacker_id) + "," +
                      std::to_string(data.attack.enemy_id) + "," +
                      std::to_string(data.attack.damage);
    break;
  case NetworkMessages::PLAYER_BUY:
    string_message += ":" + std::to_string(static_cast<int>(data.buy.item_id)) + "," +
                      std::to_string(data.buy.pos.x_pos) + "," + std::to_string(data.buy.pos.y_pos);
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

void GCNetClient::buyUnit(TroopTypes unit_type)
{
  if (tile_clicked != nullptr)
  {
    if (tile_clicked->troop_id > 0)
    {
      return;
    }
    int x_pos = static_cast<int>(tile_clicked->sprite->xPos());
    int y_pos = static_cast<int>(tile_clicked->sprite->yPos());

    Troop new_troop = Troop(unit_type, renderer, x_pos, y_pos);
    new_troop.setID(++unit_count);
    tile_clicked->troop_id = new_troop.getID();
    if (in_turn && currency >= new_troop.getCost())
    {
      currency -= new_troop.getCost();
      scene_manager.gameScreen()->closeShop();
      int client_number = static_cast<int>(client.GetUID()) - 1;
      troops[client_number].push_back(new_troop);

      Types type;
      type.buy.item_id   = unit_type;
      type.buy.pos.x_pos = x_pos;
      type.buy.pos.y_pos = y_pos;
      encodeAction(NetworkMessages::PLAYER_BUY, type);
    }
    tile_clicked = nullptr;
  }
}

void GCNetClient::addInputReader(ASGE::Input& _inputs)
{
  if (inputReader != nullptr)
  {
    delete (inputReader);
  }
  inputReader = new InputManager(_inputs);
  inputReader->setMap(map.getMap());
}
