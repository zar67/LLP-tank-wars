//
// Created by huxy on 23/02/2020.
//

#include "GCNetServer.hpp"

#include "MessageTypes.h"

#include <Engine/Logger.hpp>
#include <iostream>
#include <thread>

GCNetServer::GCNetServer() : GameComponent(ID::NETWORK_SERVER)
{
  server.Start(32488);
}

std::string GCNetServer::getIP()
{
  return server.GetClientInfo(1).ipv4;
}

void GCNetServer::update(double dt, SceneManager* scene_manager)
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
      server.SendMessageToAll(
        encodeMessage(ServerMessages::PLAYER_NUM_CHANGED, server.GetAllClients().size()));
      Logging::log(
        "New client " + info.name + " connected on ip: " + info.ipv4 + " - ID:[" +
        std::to_string(info.uid) + "]\n");
      break;
    }
    case netlib::NetworkEvent::EventType::ON_DISCONNECT:
    {
      Logging::log("Client " + std::to_string(event.senderId) + " has disconnected.\n");
      server.SendMessageToAll(
        encodeMessage(ServerMessages::PLAYER_NUM_CHANGED, server.GetAllClients().size()));
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
}

void GCNetServer::playerEndTurn()
{
  server_state = ServerState::UPDATING;
}

void GCNetServer::decodeMessage(const std::vector<char>& message)
{
  // MESSAGE FORMAT: TYPE:DATA,DATA,DATA
  auto type = static_cast<PlayerActions>(message[0] - '0');
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
  case PlayerActions::MOVE:
  {
    int unit_id = std::stoi(data[0]);
    float x_pos = std::stof(data[1]);
    float y_pos = std::stof(data[2]);

    Logging::log(
      "MESSAGE RECEIVED: MOVE ("
      "UNIT ID: " +
      std::to_string(unit_id) + ", X_POS: " + std::to_string(x_pos) +
      ", Y_POS: " + std::to_string(y_pos) + ")\n");

    break;
  }
  case PlayerActions::ATTACK:
  {
    int attacker_id = std::stoi(data[0]);
    int unit_id     = std::stoi(data[1]);
    int damage      = std::stoi(data[2]);

    Logging::log(
      "MESSAGE RECEIVED: ATTACK ("
      "ATTTACKER_ID: " +
      std::to_string(attacker_id) + ", UNIT_ID: " + std::to_string(unit_id) +
      ", DAMAGE: " + std::to_string(damage) + "\n");

    break;
  }
  case PlayerActions::BUY:
  {
    int unit_to_buy = std::stoi(data[0]);
    float x_pos     = std::stof(data[1]);
    float y_pos     = std::stof(data[2]);

    Logging::log(
      "MESSAGE RECEIVED: BUY ("
      "UNIT_TYPE: " +
      std::to_string(unit_to_buy) + ", X_POS: " + std::to_string(x_pos) +
      ", Y_POS: " + std::to_string(y_pos) + "\n");

    break;
  }
  }
}

std::vector<char> GCNetServer::encodeMessage(ServerMessages message, int data)
{
  std::string string_message = std::to_string(static_cast<int>(message));
  std::vector<char> char_message;

  string_message += ":" + std::to_string(data);

  std::copy(string_message.begin(), string_message.end(), std::back_inserter(char_message));
  return char_message;
}
