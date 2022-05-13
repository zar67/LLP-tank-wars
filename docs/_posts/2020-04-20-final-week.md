---
layout: post
title: Final Week
date: 2020-04-20 17:00:00
description: "Last Week"
published: true
category: Development
tags: 
- low-level-programming
---

It's the final week and while most of the large technical aspects of the game have been implemented we still do not have a fully working game: there is no win or lose states. 

Since last time I've implemented some more UI to help display unit information about the units as shown below:

<center>
    <img src="{{ site.baseurl }}/assets/unit_ui_stats.png" alt="unit actions" style="height: 400px;" />
</center>

The red squares show where the unit can move and on the left shows the current stats on the unit that are helpful for the player to know. Also, when an enemy unit comes into attack range, the tile turns green to show the player they're attackable.

<center>
    <img src="{{ site.baseurl }}/assets/shop_ui_stats.png" alt="unit actions" style="height: 400px;" />
</center>

I have also added to the shop, to show the stats of a unit before the player buys it. 

We've decided that in order to win the game you need to be the only player with a base camp alive. This means we need to allow the base camp to be attacked and have health. When a player's base camp loses all it's health the player loses and is out of the game, meaning that their turn should be skipped over. 

This is the next and final thing I need to implement before the deadline.