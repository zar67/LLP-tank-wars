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
  this->renderer = renderer;
  return true;
}

bool GCNetServer::update(ASGE::GameTime time)
{
  std::queue<netlib::NetworkEvent> all_events = server.GetNetworkEvents();
  while (!all_events.empty())
  {
    netlib::NetworkEvent& event = all_events.front();
    switch (event.eventType)
    {
    case netlib::NetworkEvent::EventType::ON_CONNECT:
    {
      onConnect(event);

      break;
    }
    case netlib::NetworkEvent::EventType::ON_DISCONNECT:
    {
      onDisconnect(event);
      break;
    }
    case netlib::NetworkEvent::EventType::MESSAGE:
    {
      decodeMessage(event);
      break;
    }
    }
    all_events.pop();
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  return false;
}

void GCNetServer::onDisconnect(const netlib::NetworkEvent& event)
{
  Logging::log("Client " + std::to_string(event.senderId) + " has disconnected.\n");
  server.SendMessageToAll(encodeMessage(
    NetworkMessages::PLAYER_NUM_CHANGED, std::to_string(server.GetAllClients().size())));
}

void GCNetServer::onConnect(const netlib::NetworkEvent& event)
{
  netlib::ClientInfo info = server.GetClientInfo(event.senderId);
  player_count++;
  if (server.GetAllClients().size() == 1)
  {
    server.SendMessageToAll(
      encodeMessage(NetworkMessages::PLAYER_START_TURN, std::to_string(current_turn_id)));
  }

  server.SendMessageToAll(encodeMessage(
    NetworkMessages::PLAYER_NUM_CHANGED, std::to_string(server.GetAllClients().size())));
  Logging::log(
    "New client " + info.name + " connected on ip: " + info.ipv4 + " - ID:[" +
    std::to_string(info.uid) + "]\n");
}

void GCNetServer::decodeMessage(const netlib::NetworkEvent& event)
{
  std::vector<char> message = event.data;
  auto type                 = static_cast<NetworkMessages>(message[0] - '0');

  switch (type)
  {
  case NetworkMessages::START_GAME:
  {
    server.SendMessageToAll(encodeMessage(NetworkMessages::START_GAME, ""));
    break;
  }
  case NetworkMessages::GAME_OVER:
  {
    message.push_back(static_cast<int>(event.senderId - 1) + '0');
    server.SendMessageToAll(message);
    break;
  }
  case NetworkMessages::PLAYER_END_TURN:
  {
    playerEndTurn();
    break;
  }
  case NetworkMessages::PLAYER_MOVE:
  case NetworkMessages::PLAYER_ATTACK:
  case NetworkMessages::PLAYER_BUY:
  case NetworkMessages::PLAYER_BASE_ATTACK:
  {
    message.push_back(',');
    message.push_back(static_cast<int>(event.senderId - 1) + '0');
    server.SendMessageToAllExcluding(message, event.senderId);
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
  server.SendMessageToAll(
    encodeMessage(NetworkMessages::PLAYER_START_TURN, std::to_string(current_turn_id)));

  server_state = ServerState::UPDATING;
}

std::vector<std::string> GCNetServer::getMessageData(std::vector<char> message)
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
