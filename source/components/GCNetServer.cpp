//
// Created by huxy on 23/02/2020.
//

#include "GCNetServer.hpp"
#include <Engine/Logger.hpp>
#include <iostream>
#include <thread>

GCNetServer::GCNetServer() : GameComponent(ID::NETWORK_SERVER)
{
  server.Start(32488);
}

void GCNetServer::update(double dt)
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
        Logging::log(
          "New client " + info.name + " connected on ip: " + info.ipv4 +
          " - ID:[" + std::to_string(info.uid) + "]\n");
        break;
      }
      case netlib::NetworkEvent::EventType::ON_DISCONNECT:
      {
        Logging::log(
          "Client " + std::to_string(event.senderId) + " has disconnected.\n");
        break;
      }
      case netlib::NetworkEvent::EventType::MESSAGE:
      {
        // server.SendMessageToAllExcluding(event.data, event.senderId);
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
  auto type = static_cast<MessageType>(message[0] - '0');
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
    case MessageType::MOVE:
    {
      // MOVE: PLAYER_ID, UNIT_ID, X_POS, Y_POS
      int player_id = std::stoi(data[0]);
      int unit_id   = std::stoi(data[1]);
      float x_pos   = std::stof(data[2]);
      float y_pos   = std::stof(data[3]);

      Logging::log(
        "MESSAGE RECEIVED: MOVE (PLAYER ID: " + std::to_string(player_id) +
        ", UNIT ID:" + std::to_string(unit_id) + ", X_POS: " +
        std::to_string(x_pos) + ", Y_POS: " + std::to_string(y_pos) + ")\n");

      break;
    }
    case MessageType::ATTACK:
    {
      // ATTACK: PLAYER_ID, UNIT_ID, DAMAGE
      int player_id = std::stoi(data[0]);
      int unit_id   = std::stoi(data[1]);
      int damage    = std::stoi(data[2]);

      Logging::log(
        "MESSAGE RECEIVED: ATTACK (PLAYER ID: " + std::to_string(player_id) +
        ", UNIT_ID: " + std::to_string(unit_id) +
        ", DAMAGE: " + std::to_string(damage) + "\n");

      break;
    }
    case MessageType::BUY:
    {
      // BUY: PLAYER_ID, UNIT_TYPE, X_POS, Y_POS
      int player_id = std::stoi(data[0]);
      int unit_type = std::stoi(data[1]);
      float x_pos   = std::stof(data[2]);
      float y_pos   = std::stof(data[3]);

      Logging::log(
        "MESSAGE RECEIVED: BUY (PLAYER ID: " + std::to_string(player_id) +
        ", UNIT_TYPE: " + std::to_string(unit_type) + ", X_POS: " +
        std::to_string(x_pos) + ", Y_POS: " + std::to_string(y_pos) + "\n");

      break;
    }
  }
}