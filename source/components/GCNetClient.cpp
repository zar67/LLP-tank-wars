//
// Created by huxy on 23/02/2020.
//

#include "GCNetClient.hpp"
#include <Engine/Logger.hpp>
#include <GCNetServer.hpp>
#include <thread>
GCNetClient::GCNetClient() : GameComponent(ID::NETWORK_CLIENT)
{
  // client.ConnectToIP("localhost", 32488);
  // client.ConnectToIP("164.11.76.100", 32488);
}

void GCNetClient::connectToIP(const std::string& ip)
{
  client.ConnectToIP(ip, 32488);
}

void GCNetClient::update(double dt)
{
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
        Logging::log(event.data.data());
        break;
      }
    }
    all_events.pop();
  }

  exiting = !client.IsRunning();

  std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

GCNetClient::~GCNetClient()
{
  client.Disconnect();
}

void GCNetClient::input()
{
  Types type;
  while (!exiting)
  {
    std::string input;
    std::getline(std::cin, input);

    if (input == "#move")
    {
      // EXAMPLE MOVE MESSAGE TO SERVER
      type.move.unit_id = 1;
      type.move.x_pos   = 10;
      type.move.y_pos   = 8;
      encodeData(Instructions::MOVE, type);
    }
    else if (input == "#attack")
    {
      type.attack.attacker_id = 1;
      type.attack.enenmy_id   = 4;
      type.attack.damage      = 26;
      encodeData(Instructions::ATTACK, type);
    }
    else if (input == "#buy")
    {
      type.buy.item_id   = 12;
      type.buy.pos.x_pos = 14;
      type.buy.pos.y_pos = 55;
      encodeData(Instructions::BUY, type);
    }
    else if (input == "#endturn")
    {
      endTurn();
    }
    else
    {
      std::string new_input = std::to_string(client.GetUID()) + ": " + input;
      client.SendMessageToServer(
        new_input.c_str(), static_cast<int>(new_input.size()) + 1);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}

void GCNetClient::endTurn()
{
  for (const auto& action : actions)
  {
    client.SendMessageToServer(action);
  }
  actions.clear();
}

void GCNetClient::encodeData(Instructions _instruction, Types _data)
{
  std::string string_message = std::to_string(static_cast<int>(_instruction));
  std::vector<char> message;
  switch (_instruction)
  {
    case Instructions::MOVE:
      string_message += ":" + std::to_string(_data.move.unit_id) + "," +
                        std::to_string(_data.move.x_pos) + "," +
                        std::to_string(_data.move.y_pos);
      break;
    case Instructions::ATTACK:
      string_message += ":" + std::to_string(_data.attack.attacker_id) + "," +
                        std::to_string(_data.attack.enenmy_id) + "," +
                        std::to_string(_data.attack.damage);
      break;
    case Instructions::BUY:
      string_message += ":" + std::to_string(_data.buy.item_id) + "," +
                        std::to_string(_data.buy.pos.x_pos) + "," +
                        std::to_string(_data.buy.pos.y_pos);
      break;
  }
  std::copy(
    string_message.begin(), string_message.end(), std::back_inserter(message));
  actions.push_back(message);
}