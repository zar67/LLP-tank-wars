---
layout: post
title: Setting Up Networking
date: 2020-03-12 16:00:00
description: "Networking Planning and Setup"
published: true
category: Development
tags: 
- networking
- planning
- low-level-programming
---

To start off our networking we set out the structure for sending messages across the network. As stated before we setup an enum with the message types the server and client are going to be sending and recieving, these are:

```C++
enum class NetworkMessages
{
  START_GAME         = 1,
  PLAYER_MOVE        = 2,
  PLAYER_ATTACK      = 3,
  PLAYER_BUY         = 4,
  PLAYER_END_TURN    = 5,
  PLAYER_START_TURN  = 6
};
```

So to start off with we setup the chat room tutorial that came with the networking library, this can be found [here](https://github.com/Halichoerus/NetLibrary/wiki/Chatroom-Tutorial), we did this to get our heads around the library and understand some of the functionality.

We then implemented our messages enum and setup some encode and decode functions to send the data across. Eventually we were able to setup a system that recieved the console commands #move, #attack and #buy. Then when the user entered #endturn, all the actions they had entered would be sent to the server, as seen below.

Client:
<center><img src="{{ site.baseurl }}/assets/console_client.png" alt="base uml" style="height: 200px;" /></center><br>

Server:
<center><img src="{{ site.baseurl }}/assets/console_server.png" alt="base uml" style="height: 200px;" /></center><br>

We encode these messages by using these structs to store the data we want to encode and passing that to our encodeMessage function which takes the data and turns it into a vector<char>

<center><img src="{{ site.baseurl }}/assets/action_structs_uml.png" alt="action structs" style="height: 200px;" /></center><br>

After implementing this, we realised that having some UI and something rendering to the screen would be useful moving forward, so next I will be setting up the UI system, similar to the one I created for The Shining game.