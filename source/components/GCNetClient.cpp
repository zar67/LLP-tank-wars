//
// Created by huxy on 23/02/2020.
//

#include "GCNetClient.hpp"
#include <Engine/Logger.hpp>
#include <GCNetServer.hpp>
#include <thread>
GCNetClient::GCNetClient() : GameComponent(ID::NETWORK_CLIENT)
{
  client.ConnectToIP("localhost", 32488);
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

  if (!client.IsRunning())
  {
    exiting = true;
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

GCNetClient::~GCNetClient()
{
  client.Disconnect();
}

void GCNetClient::input()
{
  while (!exiting)
  {
    std::string input;
    std::getline(std::cin, input);

    if (input == "#move")
    {
      // EXAMPLE MOVE MESSAGE TO SERVER
      Logging::log("MOVE\n");
      std::string test =
        std::to_string(static_cast<int>(GCNetServer::MessageType::MOVE)) + ":" +
        std::to_string(client.GetUID()) + "," + std::to_string(0) + "," +
        std::to_string(2) + "," + std::to_string(5);
      std::vector<char> message;
      std::copy(test.begin(), test.end(), std::back_inserter(message));
      actions.push_back(message);
    }
    else if (input == "#attack")
    {
      // EXAMPLE ATTACK MESSAGE TO SERVER
      Logging::log("ATTACK\n");
      std::string test2 =
        std::to_string(static_cast<int>(GCNetServer::MessageType::ATTACK)) +
        ":" + std::to_string(client.GetUID()) + "," + std::to_string(0) + "," +
        std::to_string(10);
      std::vector<char> message2;
      std::copy(test2.begin(), test2.end(), std::back_inserter(message2));
      actions.push_back(message2);
    }
    else if (input == "#buy")
    {
      // EXAMPLE BUY MESSAGE TO SERVER
      Logging::log("BUY\n");
      std::string test3 =
        std::to_string(static_cast<int>(GCNetServer::MessageType::BUY)) + ":" +
        std::to_string(client.GetUID()) + "," + std::to_string(0) + "," +
        std::to_string(2) + "," + std::to_string(5);
      std::vector<char> message3;
      std::copy(test3.begin(), test3.end(), std::back_inserter(message3));
      actions.push_back(message3);
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
