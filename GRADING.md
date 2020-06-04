### unamed-netgame-group-3

# The Net Game Feedback

##### Your task was to create a game using a networked turn-based mechanic.

# 

# Professional Practice

- [x] GDD present
- [x] TDD present
- [x] Followed typical studio practices
- [x] Evidence of Project Management
- [x] Exhibited game or provided Long-Play

**Notes:**

> A good set of documentation has been provided. The TDD was good, but the GDD was a little bit lacking. Your Trello board was impressive and thank you for taking the time to produce an actual watchable video of your game. I still would have preferred to of played the game with you guys in a demo session. 

# 

# Gameplay

- [x] Fully playable
- [x] Turn based system present
- [x] Has a game lobby system
- [ ] Handles disconnection
- [ ] Provides chat or VOIP communications
- [x] Polished and balanced
- [ ] Replay value present
- [x] Free of obvious bugs and/or crashes

**Notes:**

> Unfortunately, I could never get the game to start. The lobby system would load and then it would kick me back to the start menu again. 

```
New client localhost connected on ip: 127.0.0.1 - ID:[18]
Connected to the server!
Client 18 has disconnected.
```

I tried a number of attempts and even configuring the game client to only load the server if "-server" was added to the command line, but I could not get it working. My marking is thus based on your gameplay video and lacks the insight from my experience of first playing the game. This is why the demo sessions are important and it's a shame your team declined it. It's a bit disappointing for me as it appears to be quite well designed and very playable. 

# 

# Implementation

- [x] Code compliance 
- [x] Game builds
- [x] Scene manager that handles rendering and z-ordering of nodes
- [x] Input is threaded and non-blocking
- [x] Good use of object orientated design
- [ ] Well structured including separation of client/server logic
- [ ] Passes memory leak scan


**Notes:**

> Code-base is generally well laid out, easy to read and sensible written. Functions whilst often easy to read, could do with more refactoring. It's great to see you make use of the component system, but there's been some confusion here. The network components are for storing network related data and events and not the game itself. That should have been placed inside the ASGE Game executable and thus made more use of components. You really need a map and a camera component and you should make more use of unique pointers, you even have a memory leak or two present. The classes that have been identified are sensible and well written. You have made use of the STL, but again please try to avoid vectors of vectors where possible, due to extra indirection of memory. Overall, this is a very solid code-base, with some issues that could have easily been resolved holding it back. Well done. 

# 

# Marking

Your weighted grade and the breakdown is shown below.

#

| ID       | Professional <br/>20% | Gameplay<br/>(35%) | Technical<br/>(45%) | Peer       | Grade |
| -------- |:---------------------:|:------------------:|:-------------------:|:----------:| -----:|
| 18014180 |  70.00                |  65.00             |  68.50              | :grinning: | 80.00 |
| 18014341 |  70.00                |  65.00             |  68.50              | :grinning: | 80.00 | 
| 18026856 |  70.00                |  65.00             |  68.50              | :neutral:  | 62.00 |
| 18013287 |  70.00                |  65.00             |  68.50              | :neutral:  | 62.00 |
| 18022598 |  70.00                |  65.00             |  68.50              | :skull:    | 0.00  |

This assignment contributes **50%** of your portfolio mark. 

> You can find more information on breakdown and weightings on [Blackboard]. 

#### [Blackboard]: <https://blackboard.uwe.ac.uk>
