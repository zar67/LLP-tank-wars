//
// Created by huxy on 23/02/2020.
//

#include "GCNetClient.hpp"
GCNetClient::GCNetClient() : GameComponent(ID::NETWORK_CLIENT)
{
  client.ConnectToIP("localhost", 32488);
}
void GCNetClient::update(double dt) {}

GCNetClient::~GCNetClient()
{
  client.Disconnect();
}
