//
// Created by huxy on 23/02/2020.
//

#include "GCNetClient.hpp"

#include <Engine/Logger.hpp>
#include <thread>

GCNetClient::GCNetClient() : GameComponent(ID::NETWORK_CLIENT)
{
  // client.ConnectToIP("localhost", 32488);
  // client.ConnectToIP("164.11.76.100", 32488);
}

GCNetClient::~GCNetClient()
{
  client.Disconnect();
}

bool GCNetClient::init(ASGE::Renderer* renderer, int font_index)
{
  return scene_manager.init(renderer, font_index);
}

bool GCNetClient::update(
  double dt,
  const ASGE::Point2D& cursor_pos,
  bool click,
  bool key_pressed,
  int key)
{
  if (updateUI(cursor_pos, click, key_pressed, key))
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
      std::thread tr(&GCNetClient::input, this);
      tr.detach();
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

bool GCNetClient::updateUI(const ASGE::Point2D& cursor_pos, bool click, bool key_pressed, int key)
{
  UIElement::MenuItem item = scene_manager.update(in_turn, cursor_pos, click, key_pressed, key);

  switch (item)
  {
  case (UIElement::MenuItem::EXIT_GAME):
  {
    return true;
  }
  case (UIElement::MenuItem::START_GAME):
  {
    if (canStartGame())
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
    buyUnit(0);
    break;
  }
  case (UIElement::MenuItem::BUY_UNIT_1):
  {
    buyUnit(1);
    break;
  }
  case (UIElement::MenuItem::BUY_UNIT_2):
  {
    buyUnit(2);
    break;
  }
  case (UIElement::MenuItem::BUY_UNIT_3):
  {
    buyUnit(3);
    break;
  }
  }

  return false;
}

void GCNetClient::render(ASGE::Renderer* renderer)
{
  scene_manager.render(renderer, currency);
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
    can_start = player_num >= 2;
    break;
  }
  case (NetworkMessages::PLAYER_END_TURN):
  {
    endTurn();
    break;
  }
  case (NetworkMessages::PLAYER_START_TURN):
  {
    in_turn = true;
    break;
  }
  }
}

void GCNetClient::encodeAction(NetworkMessages instruction, ActionTypes data)
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
    string_message += ":" + std::to_string(data.buy.item_id) + "," +
                      std::to_string(data.buy.pos.x_pos) + "," + std::to_string(data.buy.pos.y_pos);
    break;
  }
  std::copy(string_message.begin(), string_message.end(), std::back_inserter(message));
  actions.push_back(message);
}

void GCNetClient::input()
{
  ActionTypes type;
  while (!exiting)
  {
    std::string input;
    std::getline(std::cin, input);

    if (input == "#move")
    {
      type.move.unit_id = 1;
      type.move.x_pos   = 10;
      type.move.y_pos   = 8;
      encodeAction(NetworkMessages::PLAYER_MOVE, type);
    }
    else if (input == "#attack")
    {
      type.attack.attacker_id = 1;
      type.attack.enemy_id    = 4;
      type.attack.damage      = 26;
      encodeAction(NetworkMessages::PLAYER_ATTACK, type);
    }
    else if (input == "#buy")
    {
      type.buy.item_id   = 12;
      type.buy.pos.x_pos = 14;
      type.buy.pos.y_pos = 55;
      encodeAction(NetworkMessages::PLAYER_BUY, type);
    }
    else if (input == "#endturn")
    {
      endTurn();
    }
    else
    {
      std::string new_input = std::to_string(client.GetUID()) + ": " + input;
      client.SendMessageToServer(new_input.c_str(), static_cast<int>(new_input.size()) + 1);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
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

bool GCNetClient::canStartGame()
{
  return can_start;
}

void GCNetClient::startGame()
{
  std::string string_message = std::to_string(static_cast<int>(NetworkMessages::START_GAME));
  std::vector<char> message;
  std::copy(string_message.begin(), string_message.end(), std::back_inserter(message));

  client.SendMessageToServer(message);
}

void GCNetClient::buyUnit(int unit_id)
{
  if (in_turn && currency >= 10)  // TODO: CHANGE TO UNIT AMOUNT
  {
    currency -= 10;  // TODO: CHANGE TO UNIT AMOUNT
    scene_manager.gameScreen()->closeShop();

    // TODO: APPEND NEW UNIT TO UNITS

    ActionTypes type;
    type.buy.item_id   = 12;
    type.buy.pos.x_pos = 14;
    type.buy.pos.y_pos = 55;
    encodeAction(NetworkMessages::PLAYER_BUY, type);

    Logging::log("BOUGHT UNIT " + std::to_string(unit_id));
  }
}
