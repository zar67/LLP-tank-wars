//
// Created by huxy on 23/02/2020.
//

#include "components/GCNetServer.hpp"

#include "gamedata/MessageTypes.h"

#include <Engine/Logger.hpp>
#include <iostream>
#include <thread>

GCNetServer::GCNetServer() : GameComponent(ID::NETWORK_SERVER)
{
  server.Start(32488);
}

bool GCNetServer::init(ASGE::Renderer* renderer, int font_index)
{
  return true;
}

bool GCNetServer::update(
  double dt,
  const ASGE::Point2D& cursor_pos,
  bool click,
  bool key_pressed,
  int key)
{
  std::queue<netlib::NetworkEvent> all_events = server.GetNetworkEvents();
  while (!all_events.empty())
  {
    netlib::NetworkEvent& event = all_events.front();
    switch (event.eventType)
    {
    case netlib::NetworkEvent::EventType::ON_CONNECT:
    {
      netlib::ClientInfo info = server.GetClientInfo(event.senderId);

      if (server.GetAllClients().size() == 1)
      {
        server.SendMessageTo(encodeMessage(NetworkMessages::PLAYER_START_TURN, ""), 1);
      }

      server.SendMessageToAll(encodeMessage(
        NetworkMessages::PLAYER_NUM_CHANGED, std::to_string(server.GetAllClients().size())));
      Logging::log(
        "New client " + info.name + " connected on ip: " + info.ipv4 + " - ID:[" +
        std::to_string(info.uid) + "]\n");
      break;
    }
    case netlib::NetworkEvent::EventType::ON_DISCONNECT:
    {
      Logging::log("Client " + std::to_string(event.senderId) + " has disconnected.\n");
      server.SendMessageToAll(encodeMessage(
        NetworkMessages::PLAYER_NUM_CHANGED, std::to_string(server.GetAllClients().size())));
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
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  return false;
}

void GCNetServer::decodeMessage(const std::vector<char>& message)
{
  // MESSAGE FORMAT: TYPE:DATA,DATA,DATA
  auto type = static_cast<NetworkMessages>(message[0] - '0');
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

  switch (type)
  {
  case NetworkMessages::START_GAME:
  {
    server.SendMessageToAll(encodeMessage(NetworkMessages::START_GAME, ""));
    break;
  }
  case NetworkMessages::PLAYER_END_TURN:
  {
    playerEndTurn();
    break;
  }
  case NetworkMessages::PLAYER_MOVE:
  {
    int unit_id = std::stoi(data[0]);
    float x_pos = std::stof(data[1]);
    float y_pos = std::stof(data[2]);

    // TODO: Move The Unit
    Logging::log(
      "MESSAGE RECEIVED: PLAYER_MOVE ("
      "UNIT ID: " +
      std::to_string(unit_id) + ", X_POS: " + std::to_string(x_pos) +
      ", Y_POS: " + std::to_string(y_pos) + ")\n");

    break;
  }
  case NetworkMessages::PLAYER_ATTACK:
  {
    int attacker_id = std::stoi(data[0]);
    int unit_id     = std::stoi(data[1]);
    int damage      = std::stoi(data[2]);

    // TODO: Attack The Unit
    Logging::log(
      "MESSAGE RECEIVED: PLAYER_ATTACK ("
      "ATTTACKER_ID: " +
      std::to_string(attacker_id) + ", UNIT_ID: " + std::to_string(unit_id) +
      ", DAMAGE: " + std::to_string(damage) + "\n");

    break;
  }
  case NetworkMessages::PLAYER_BUY:
  {
    int unit_to_buy = std::stoi(data[0]);
    float x_pos     = std::stof(data[1]);
    float y_pos     = std::stof(data[2]);

    // TODO: Create The Unit
    Logging::log(
      "MESSAGE RECEIVED: BUY ("
      "UNIT_TYPE: " +
      std::to_string(unit_to_buy) + ", X_POS: " + std::to_string(x_pos) +
      ", Y_POS: " + std::to_string(y_pos) + "\n");

    break;
  }
  }
}

std::vector<char> GCNetServer::encodeMessage(NetworkMessages message, const std::string& data)
{
  std::string string_message = std::to_string(static_cast<int>(message));
  std::vector<char> char_message;

  string_message += ":" + data;

  std::copy(string_message.begin(), string_message.end(), std::back_inserter(char_message));
  return char_message;
}

std::string GCNetServer::getIP()
{
  return server.GetClientInfo(1).ipv4;
}

void GCNetServer::playerEndTurn()
{
  current_turn_id %= server.GetAllClients().size();
  current_turn_id += 1;
  server.SendMessageTo(encodeMessage(NetworkMessages::PLAYER_START_TURN, "0"), current_turn_id);

  server_state = ServerState::UPDATING;
}
