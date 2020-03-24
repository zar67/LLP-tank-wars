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

void GCNetClient::update(double dt, SceneManager* scene_manager)
{
  scene_manager->gameScreen()->setInTurn(in_turn);
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
      decodeMessage(scene_manager, event.data);
      break;
    }
    }
    all_events.pop();
  }

  exiting = !client.IsRunning();
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

void GCNetClient::decodeMessage(SceneManager* scene_manager, const std::vector<char>& message)
{
  auto type = static_cast<NetworkMessages>(message[0] - '0');

  switch (type)
  {
  case (NetworkMessages::START_GAME):
  {
    scene_manager->screenOpen(SceneManager::Screens::GAME);
    break;
  }
  case (NetworkMessages::PLAYER_NUM_CHANGED):
  {
    int player_num = static_cast<int>(message[2] - '0');
    scene_manager->lobbyScreen()->setPlayerNumber(player_num);
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

bool GCNetClient::connectToIP(const std::string& ip)
{
  return client.ConnectToIP(ip, 32488);
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
      // EXAMPLE PLAYER_MOVE MESSAGE TO SERVER
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
