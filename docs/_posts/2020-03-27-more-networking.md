---
layout: post
title: More Networking
date: 2020-03-27 16:00:00
description: "Refining and Improving Our Networking"
published: true
category: Development
tags: 
- networking
- planning
- low-level-programming
---

So far, I have setup the player turns, sending actions to clients, and some small features.

To setup the player turns, I added a new int on the server to hold the id of the player who's current turn it is. I also added a boolean onto each client which states whether that player is currently on their turn. I disable any actions by ensuring that in_turn is true before the player can buy, attack or move. I have also disabled the end turn button and it will only appear if it is currently the player's turn, since there is no point in rendering it otherwise. 

For example this is currently the function for buying a unit (called when a unit is clicked on in the shop):

```C++
void GCNetClient::buyUnit(TroopTypes unit_type)
{
  int x_pos = 500;
  int y_pos = 500;

  Troop new_troop = Troop(unit_type, renderer, x_pos, y_pos);

  if (in_turn && currency >= new_troop.getCost())
  {
    currency -= new_troop.getCost();
    scene_manager.gameScreen()->closeShop();
    int client_number = static_cast<int>(client.GetUID()) - 1;
    troops[client_number].push_back(new_troop);

    Types type;
    type.buy.item_id   = unit_type;
    type.buy.pos.x_pos = x_pos;
    type.buy.pos.y_pos = y_pos;
    encodeAction(NetworkMessages::PLAYER_BUY, type);
  }
}
```

Aaron has previously setup a Troop class, allowing different statistics based on the TroopType that is passed into the constructor. 
After a troop is successfully bought, we encode the action ready to be sent to the server when the end turn button is pressed. The end turn function currently looks like this:

```C++
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
```

As you can see each action that has taken place is sent to the server (and then sent on to all the other clients) and the actions vector is cleared of the actions. After this, we also tell the server to end the current player's turn, the server then calculates the id of the new player and sends a PLAYER_START_TURN message to everyone. The PLAYER_START_TURN message is send with the id of the player whose turn it is, and this is sent to everyone so that we can display who's turn it is on the the screen as shown below:

<center>
    <img src="{{ site.baseurl }}/assets/player_1.png" alt="player 1" style="height: 200px;" />
    <img src="{{ site.baseurl }}/assets/player_2.png" alt="player 2" style="height: 200px;" />    
</center><br>