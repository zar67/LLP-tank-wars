---
layout: post
title: Post Mortem
date: 2020-04-30 09:00:00
description: "Post Mortem"
published: true
category: Development
tags: 
- post-mortem
- low-level-programming
---

After successfully implementing the last few features our game is done! We managed to test and bug fix a little before the deadline to make sure everything was working and in general I am pleased with the outcome of this game. Learning the networking and the new structure for this type of game was challenging and I'm proud we managed to learn more about networking and threading. 

Things that went well:
* We successfully learnt about and implemented a networked multiplayer game where players on the same internet connected can connect and play.
* Successfully created a strategic turn based game for 2-4 players.

What didn't go well:
* Slightly buggy networking: We realised too late into development that using the clientUID wasn't the best way to identify different players, since if a client disconnects and reconnects they have a different UID, meaning that our system breaks. 
* We didn't implement everything that we wanted to, some of the extra features we wanted were: animations, end of turn transitions and buildings to generate more currency per turn.
* The game could do with more polishing, like animations and more gameplay features to make the game more competitive and fun.

Overall I'm pretty happy with the game we've created, we managed our time better on this task and managed to implement all the major features needed for the game. 

Here is a demo video of our game:

<center>
<iframe width="592" height="416" src="https://www.youtube.com/embed/zkCfj4FR3hE" frameborder="0" allowfullscreen></iframe>
</center>

