//
// Created by huxy on 23/02/2020.
//

#ifndef NETGAME_GCNETCLIENT_HPP
#define NETGAME_GCNETCLIENT_HPP
#include "GameComponent.hpp"
#include <NetLib/ClientConnection.h>
class GCNetClient : public GameComponent
{
 public:
  GCNetClient();
  ~GCNetClient() override;
  void update(double dt) override;

  GCNetClient(const GCNetClient&) = delete;
  GCNetClient& operator=(const GCNetClient&) = delete;

 private:
  netlib::ClientConnection client;
};

#endif // NETGAME_GCNETCLIENT_HPP
