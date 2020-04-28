//
// Created by huxy on 23/02/2020.
//

#include "components/GCNetClient.hpp"

#include <Engine/Logger.hpp>
#include <thread>

GCNetClient::GCNetClient() : GameComponent(ID::NETWORK_CLIENT)
{
  cam = new ASGE::Camera2D(
    ASGE::Point2D(0, 0),
    static_cast<float>(ASGE::SETTINGS.window_width),
    static_cast<float>(ASGE::SETTINGS.window_height));
  cam->lookAt(ASGE::Point2D(
    -static_cast<float>(ASGE::SETTINGS.window_width) / 2,
    -static_cast<float>(ASGE::SETTINGS.window_height) / 2));
}

GCNetClient::~GCNetClient()
{
  if (input_reader != nullptr)
  {
    input_reader->exitInputThread();
    delete (input_reader);
    input_reader = nullptr;
  }

  for (const auto& player : troops)
  {
    for (auto* troop : player)
    {
      delete troop;
      troop = nullptr;
    }
  }
  troops.clear();

  client.Disconnect();
}

bool GCNetClient::init(ASGE::Renderer* renderer, int font_index)
{
  this->renderer   = renderer;
  this->font_index = font_index;

  map.init(ASGE::SETTINGS.window_width, ASGE::SETTINGS.window_height);
  map.generateMap(renderer);

  if (!audio_manager.audioSetUp())
  {
    return false;
  }
  audio_manager.playgameMenu();

  return scene_manager.init(renderer, &audio_manager, font_index);
}

bool GCNetClient::update(ASGE::GameTime time)
{
  if (updateUI())
  {
    return true;
  }

  cam->update(time);

  std::queue<netlib::NetworkEvent> all_events = client.GetNetworkEvents();
  while (!all_events.empty())
  {
    netlib::NetworkEvent& event = all_events.front();
    switch (event.eventType)
    {
    case netlib::NetworkEvent::EventType::ON_CONNECT:
    {
      Logging::log("Connected to the server!\n");

      if (static_cast<int>(client.GetUID()) > 4)
      {
        scene_manager.screenOpen(SceneManager::Screens::MAIN_MENU);
        client.Disconnect();
      }
      else
      {
        scene_manager.gameScreen()->initShop(
          renderer, font_index, static_cast<int>(client.GetUID()));
        input_reader->setBaseColours(clientIndexNumber());
      }

      break;
    }
    case netlib::NetworkEvent::EventType::ON_DISCONNECT:
    {
      Logging::log("Disconnected from server! Shutting down...\n");
      break;
    }
    case netlib::NetworkEvent::EventType::MESSAGE:
    {
      decodeMessage(event.data);
      break;
    }
    }
    all_events.pop();
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  return false;
}

bool GCNetClient::updateUI()
{
  std::array<int, 2> cam_pos = {static_cast<int>(cam->getView().x),
                                static_cast<int>(cam->getView().y)};
  UIElement::MenuItem item   = scene_manager.update(input_reader, cam_pos);

  switch (item)
  {
  case (UIElement::MenuItem::EXIT_GAME):
  {
    return true;
  }
  case (UIElement::MenuItem::OPEN_MENU):
  {
    // Stop server
    client.Disconnect();
    reset();
    break;
  }
  case (UIElement::MenuItem::START_GAME):
  {
    if (can_start)
    {
      initGame();
      startGame();
    }
    break;
  }
  case (UIElement::MenuItem::HOST_GAME):
  {
    client.ConnectToIP("localHost", 32488);
    break;
  }
  case (UIElement::MenuItem::CONNECT_TO_IP):
  {
    if (client.ConnectToIP(scene_manager.joinScreen()->getIP(), 32488))
    {
      scene_manager.screenOpen(SceneManager::Screens::LOBBY);
    }
    else
    {
      scene_manager.joinScreen()->displayConnectionError();
    }
    break;
  }
  case (UIElement::MenuItem::END_TURN):
  {
    endTurn();
    break;
  }
  case (UIElement::MenuItem::BUY_NORMAL_TANK):
  {
    input_reader->deselectTile();
    shop_unit_selected = TroopTypes::NORMAL_TANK;
    break;
  }
  case (UIElement::MenuItem::BUY_BIG_TANK):
  {
    input_reader->deselectTile();
    shop_unit_selected = TroopTypes::BIG_TANK;
    break;
  }
  case (UIElement::MenuItem::BUY_LARGE_TANK):
  {
    input_reader->deselectTile();
    shop_unit_selected = TroopTypes::LARGE_TANK;
    break;
  }
  case (UIElement::MenuItem::BUY_HUGE_TANK):
  {
    input_reader->deselectTile();
    shop_unit_selected = TroopTypes::HUGE_TANK;
    break;
  }
  case (UIElement::MenuItem::MAP_CLICK):
  {
    input_reader->setClickedMap(
      clientIndexNumber(),
      troops[clientIndexNumber()],
      *input_reader->mouseClicked(),
      input_reader->mousePos().x,
      input_reader->mousePos().y);

    TileData* tile_clicked       = input_reader->tileClicked();
    TileData* previously_clicked = input_reader->previousTileClicked();

    if (in_turn && tile_clicked != nullptr && time_units_spent < max_time_units)
    {
      if (!tile_clicked->is_base && shop_unit_selected != TroopTypes::NONE)
      {
        buyUnit(tile_clicked, shop_unit_selected);
        shop_unit_selected = TroopTypes::NONE;
      }
      else if (
        !tile_clicked->is_base && previously_clicked != nullptr &&
        previously_clicked->tile_id != tile_clicked->tile_id &&
        previously_clicked->troop_player_id == clientIndexNumber() &&
        previously_clicked->troop_id >= 0 &&
        !getTroop(clientIndexNumber(), previously_clicked->troop_id)->getBoughtThisTurn() &&
        tile_clicked->troop_id < 0)
      {
        moveUnit(tile_clicked, previously_clicked);
      }
      else if (
        !tile_clicked->is_base && previously_clicked != nullptr &&
        previously_clicked->tile_id != tile_clicked->tile_id &&
        previously_clicked->troop_player_id == clientIndexNumber() &&
        previously_clicked->troop_id >= 0 &&
        !getTroop(clientIndexNumber(), previously_clicked->troop_id)->getBoughtThisTurn() &&
        !getTroop(clientIndexNumber(), previously_clicked->troop_id)->getAttackedThisTurn() &&
        tile_clicked->troop_player_id != clientIndexNumber() && tile_clicked->troop_id >= 0)
      {
        attackUnit(tile_clicked, previously_clicked);
      }
      else if (
        previously_clicked != nullptr && previously_clicked->tile_id != tile_clicked->tile_id &&
        previously_clicked->troop_player_id == clientIndexNumber() &&
        previously_clicked->troop_id >= 0 &&
        !getTroop(clientIndexNumber(), previously_clicked->troop_id)->getBoughtThisTurn() &&
        !getTroop(clientIndexNumber(), previously_clicked->troop_id)->getAttackedThisTurn() &&
        tile_clicked->player_base_id != -1)
      {
        attackBaseCamp(tile_clicked, previously_clicked);
      }
    }

    input_reader->unlockPreviousTile();
    input_reader->unlockTile();

    break;
  }
  }

  return false;
}

void GCNetClient::render()
{
  renderer->setProjectionMatrix(cam->getView());

  if (scene_manager.screenOpen() == SceneManager::Screens::GAME)
  {
    TileData* tile_clicked = input_reader->tileClicked();
    if (tile_clicked != nullptr && tile_clicked->troop_id != -1)
    {
      scene_manager.renderGameScreen(
        renderer,
        time_units_spent,
        current_turn_id,
        in_turn,
        alive,
        getTroop(tile_clicked->troop_player_id, tile_clicked->troop_id),
        troops,
        &map,
        currency);
    }
    else
    {
      scene_manager.renderGameScreen(
        renderer, time_units_spent, current_turn_id, in_turn, alive, nullptr, troops, &map, currency);
    }
    input_reader->unlockTile();
  }
  else
  {
    scene_manager.render(renderer);
  }
}

void GCNetClient::decodeMessage(const std::vector<char>& message)
{
  auto type = static_cast<NetworkMessages>(message[0] - '0');

  switch (type)
  {
  case (NetworkMessages::START_GAME):
  {
    initGame();
    break;
  }
  case (NetworkMessages::GAME_OVER):
  {
    scene_manager.gameOverScreen()->setWinningValues(static_cast<int>(message[1] - '0') + 1);
    scene_manager.screenOpen(SceneManager::Screens::GAME_OVER);
    cam->lookAt(ASGE::Point2D(
      -static_cast<float>(ASGE::SETTINGS.window_width) / 2,
      -static_cast<float>(ASGE::SETTINGS.window_height) / 2));
    break;
  }
  case (NetworkMessages::PLAYER_NUM_CHANGED):
  {
    num_connected_players = static_cast<int>(message[2] - '0');
    scene_manager.lobbyScreen()->setPlayerNumber(num_connected_players);
    can_start = num_connected_players >= 2;
    break;
  }
  case (NetworkMessages::PLAYER_END_TURN):
  {
    endTurn();
    break;
  }
  case (NetworkMessages::PLAYER_START_TURN):
  {
    current_turn_id = static_cast<int>(message[2] - '0');
    if (current_turn_id == client.GetUID())
    {
      if (alive)
      {
        in_turn = true;
        currency += 10;
      }
      else
      {
        endTurn();
      }
    }
    break;
  }
  default:
  {
    handleActions(message);
    break;
  }
  }
}

void GCNetClient::handleActions(const std::vector<char>& message)
{
  // MESSAGE FORMAT: TYPE:DATA,DATA,DATA:SENDER_ID
  auto type = static_cast<NetworkMessages>(message[0] - '0');

  switch (type)
  {
  case NetworkMessages::PLAYER_MOVE:
  {
    std::vector<std::string> data = getMessageData(message);
    int unit_id                   = std::stoi(data[0]);
    int current_tile_id           = std::stoi(data[1]);
    int new_tile_id               = std::stoi(data[2]);
    int sender_id                 = std::stoi(data[3]);

    TileData* current_tile = map.getTile(current_tile_id);
    TileData* new_tile     = map.getTile(new_tile_id);

    ASGE::Sprite* sprite = getTroop(sender_id, unit_id)->getSpriteComponent()->getSprite();
    sprite->xPos(new_tile->sprite->xPos() + new_tile->sprite->width() / 2 - sprite->width() / 2);
    sprite->yPos(new_tile->sprite->yPos() + new_tile->sprite->height() / 2 - sprite->height() / 2);

    new_tile->troop_player_id     = sender_id;
    new_tile->troop_id            = unit_id;
    current_tile->troop_player_id = -1;
    current_tile->troop_id        = -1;

    break;
  }
  case NetworkMessages::PLAYER_ATTACK:
  {
    std::vector<std::string> data = getMessageData(message);
    int tile_id                   = std::stoi(data[0]);
    int damage                    = std::stoi(data[1]);
    int sender_id                 = std::stoi(data[2]);

    TileData* tile     = map.getTile(tile_id);
    Troop* other_troop = getTroop(tile->troop_player_id, tile->troop_id);

    other_troop->takeDamage(damage);

    if (other_troop->getHealth() <= 0)
    {
      int troop_id = tile->troop_id;

      auto it = std::find_if(
        troops[tile->troop_player_id].begin(),
        troops[tile->troop_player_id].end(),
        [troop_id](const Troop* troop) { return troop->getID() == troop_id; });

      if (it != troops[tile->troop_player_id].end())
      {
        troops[tile->troop_player_id].erase(it);
      }

      tile->troop_id        = -1;
      tile->troop_player_id = -1;
    }

    break;
  }
  case NetworkMessages::PLAYER_BASE_ATTACK:
  {
    handleBaseAttack(message);
    break;
  }
  case NetworkMessages::PLAYER_BUY:
  {
    std::vector<std::string> data = getMessageData(message);
    auto unit_to_buy              = static_cast<TroopTypes>(std::stoi(data[0]));
    int unit_id                   = std::stoi(data[1]);
    int tile_id                   = std::stoi(data[2]);
    int sender_id                 = std::stoi(data[3]);

    TileData* tile = map.getTile(tile_id);

    int x_pos = static_cast<int>(tile->sprite->xPos() + tile->sprite->width() / 2);
    int y_pos = static_cast<int>(tile->sprite->yPos() + tile->sprite->height() / 2);

    troops[sender_id].emplace_back(
      new Troop(unit_id, unit_to_buy, renderer, x_pos, y_pos, sender_id + 1, false));

    tile->troop_id        = unit_id;
    tile->troop_player_id = sender_id;

    break;
  }
  }
}

void GCNetClient::handleBaseAttack(const std::vector<char>& message)
{
  std::vector<std::string> data = getMessageData(message);
  int tile_id                   = std::stoi(data[0]);
  int damage                    = std::stoi(data[1]);
  int sender_id                 = std::stoi(data[2]);

  TileData* tile = map.getTile(tile_id);

  tile->base_health -= damage;

  if (tile->base_health <= 0)
  {
    if (tile->player_base_id == clientIndexNumber())
    {
      alive = false;
    }

    players_alive.at(tile->player_base_id) = false;
    tile->is_base                          = false;
    tile->player_base_id                   = -1;
    input_reader->setBaseColours(clientIndexNumber());
  }
}

void GCNetClient::encodeAction(NetworkMessages instruction, Types data)
{
  std::string string_message = std::to_string(static_cast<int>(instruction));
  std::vector<char> message;
  switch (instruction)
  {
  case NetworkMessages::PLAYER_MOVE:
  {
    string_message += ":" + std::to_string(data.move.unit_index) + "," +
                      std::to_string(data.move.current_tile_id) + "," +
                      std::to_string(data.move.new_tile_id);
    break;
  }
  case NetworkMessages::PLAYER_ATTACK:
  case NetworkMessages::PLAYER_BASE_ATTACK:
  {
    string_message +=
      ":" + std::to_string(data.attack.tile_id) + "," + std::to_string(data.attack.damage);
    break;
  }
  case NetworkMessages::PLAYER_BUY:
  {
    string_message += ":" + std::to_string(static_cast<int>(data.buy.unit_type)) + "," +
                      std::to_string(data.buy.unit_id) + "," + std::to_string(data.buy.tile_id);
    break;
  }
  }
  std::copy(string_message.begin(), string_message.end(), std::back_inserter(message));
  actions.push_back(message);
  time_units_spent += 1;
}

std::vector<std::string> GCNetClient::getMessageData(std::vector<char> message)
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

void GCNetClient::endTurn()
{
  if (in_turn)
  {
    input_reader->deselectTile();
    in_turn = false;

    time_units_spent = 0;

    for (const auto& action : actions) { client.SendMessageToServer(action); }
    actions.clear();

    for (auto* troop : units_bought_this_turn) { troop->setBoughtThisTurn(false); }
    units_bought_this_turn.clear();

    for (auto* troop : units_attacked_this_turn) { troop->setAttackedThisTurn(false); }
    units_attacked_this_turn.clear();
  }

  std::string string_message = std::to_string(static_cast<int>(NetworkMessages::PLAYER_END_TURN));
  std::vector<char> message;
  std::copy(string_message.begin(), string_message.end(), std::back_inserter(message));

  client.SendMessageToServer(message);
}

void GCNetClient::startGame()
{
  std::string string_message = std::to_string(static_cast<int>(NetworkMessages::START_GAME));
  std::vector<char> message;
  std::copy(string_message.begin(), string_message.end(), std::back_inserter(message));

  client.SendMessageToServer(message);
}

Troop* GCNetClient::getTroop(int player_id, int troop_id)
{
  auto it = std::find_if(
    troops[player_id].begin(), troops[player_id].end(), [troop_id](const Troop* troop) {
      return troop->getID() == troop_id;
    });

  if (it != troops[player_id].end())
  {
    return *it;
  }

  return nullptr;
}

void GCNetClient::buyUnit(TileData* tile_clicked, TroopTypes unit_type)
{
  if (tile_clicked->troop_id > -1 || !map.inRangeOfBase(*tile_clicked, clientIndexNumber()))
  {
    return;
  }

  int x_pos = static_cast<int>(tile_clicked->sprite->xPos() + tile_clicked->sprite->width() / 2);
  int y_pos = static_cast<int>(tile_clicked->sprite->yPos() + tile_clicked->sprite->height() / 2);

  troops[clientIndexNumber()].emplace_back(new Troop(
    unit_count++, unit_type, renderer, x_pos, y_pos, static_cast<int>(client.GetUID()), true));
  Troop* new_troop = troops[clientIndexNumber()].back();

  if (currency >= new_troop->getCost())
  {
    units_bought_this_turn.emplace_back(new_troop);
    audio_manager.playbuyUnit();

    currency -= new_troop->getCost();
    scene_manager.gameScreen()->closeShop();

    tile_clicked->troop_id        = new_troop->getID();
    tile_clicked->troop_player_id = clientIndexNumber();

    Types type;
    type.buy.unit_type = unit_type;
    type.buy.unit_id   = new_troop->getID();
    type.buy.tile_id   = tile_clicked->tile_id;
    encodeAction(NetworkMessages::PLAYER_BUY, type);
  }
  else
  {
    troops[clientIndexNumber()].pop_back();
  }

  input_reader->deselectTile();
}

void GCNetClient::moveUnit(TileData* tile_clicked, TileData* previously_clicked)
{
  int range = getTroop(clientIndexNumber(), previously_clicked->troop_id)->getMovementRange();

  if (
    tile_clicked != nullptr &&
    map.tileInRange(previously_clicked->tile_id, tile_clicked->tile_id, range))
  {
    audio_manager.playMovement();

    ASGE::Sprite* sprite =
      getTroop(clientIndexNumber(), previously_clicked->troop_id)->getSpriteComponent()->getSprite();
    sprite->xPos(
      tile_clicked->sprite->xPos() + tile_clicked->sprite->width() / 2 - sprite->width() / 2);
    sprite->yPos(
      tile_clicked->sprite->yPos() + tile_clicked->sprite->height() / 2 - sprite->height() / 2);

    tile_clicked->troop_id              = previously_clicked->troop_id;
    tile_clicked->troop_player_id       = clientIndexNumber();
    previously_clicked->troop_player_id = -1;
    previously_clicked->troop_id        = -1;

    Types type;
    type.move.unit_index      = tile_clicked->troop_id;
    type.move.current_tile_id = previously_clicked->tile_id;
    type.move.new_tile_id     = tile_clicked->tile_id;
    encodeAction(NetworkMessages::PLAYER_MOVE, type);
  }
}

void GCNetClient::attackUnit(TileData* tile_clicked, TileData* previously_clicked)
{
  Troop* owned_troop = getTroop(clientIndexNumber(), previously_clicked->troop_id);
  Troop* other_troop = getTroop(tile_clicked->troop_player_id, tile_clicked->troop_id);

  units_attacked_this_turn.emplace_back(owned_troop);
  owned_troop->setAttackedThisTurn(true);
  other_troop->takeDamage(owned_troop->getAttackDamage());

  if (other_troop->getHealth() <= 0)
  {
    audio_manager.playExplosion();
    auto it = std::find(
      troops[tile_clicked->troop_player_id].begin(),
      troops[tile_clicked->troop_player_id].end(),
      getTroop(tile_clicked->troop_player_id, tile_clicked->troop_id));
    troops[tile_clicked->troop_player_id].erase(it);

    tile_clicked->troop_id        = -1;
    tile_clicked->troop_player_id = -1;
  }
  else
  {
    audio_manager.playShoot();
  }

  Types type;
  type.attack.tile_id = tile_clicked->tile_id;
  type.attack.damage  = owned_troop->getAttackDamage();
  encodeAction(NetworkMessages::PLAYER_ATTACK, type);
}

void GCNetClient::attackBaseCamp(TileData* tile_clicked, TileData* previously_clicked)
{
  Troop* owned_troop = getTroop(clientIndexNumber(), previously_clicked->troop_id);
  units_attacked_this_turn.emplace_back(owned_troop);
  owned_troop->setAttackedThisTurn(true);

  tile_clicked->base_health -= owned_troop->getAttackDamage();

  if (tile_clicked->base_health <= 0)
  {
    players_alive.at(tile_clicked->player_base_id) = false;
    tile_clicked->is_base                          = false;
    tile_clicked->player_base_id                   = -1;

    bool game_won = true;
    for (int i = 0; i < num_connected_players; i++)
    {
      if (players_alive.at(i) && i != clientIndexNumber())
      {
        game_won = false;
        break;
      }
    }

    if (game_won)
    {
      std::string string_message = std::to_string(static_cast<int>(NetworkMessages::GAME_OVER));
      std::vector<char> server_message;
      std::copy(string_message.begin(), string_message.end(), std::back_inserter(server_message));

      client.SendMessageToServer(server_message);
    }
  }

  Types type;
  type.attack.tile_id = tile_clicked->tile_id;
  type.attack.damage  = owned_troop->getAttackDamage();
  encodeAction(NetworkMessages::PLAYER_BASE_ATTACK, type);
}

void GCNetClient::addInputReader(ASGE::Input& _inputs)
{
  if (input_reader != nullptr)
  {
    delete (input_reader);
  }
  input_reader = new InputManager(_inputs, &audio_manager, cam, &map);
}

int GCNetClient::clientIndexNumber()
{
  return static_cast<int>(client.GetUID()) - 1;
}

void GCNetClient::initGame()
{
  map.setBaseCamps(num_connected_players);
  scene_manager.screenOpen(SceneManager::Screens::GAME);
  audio_manager.stopAudio();
  audio_manager.playBackgroundMusic();
  input_reader->setInGame(true);
  cam->lookAt(
    ASGE::Point2D(cam_starting_x[clientIndexNumber()], cam_starting_y[clientIndexNumber()]));
}
void GCNetClient::reset()
{
  can_start        = true;
  in_turn          = false;
  current_turn_id  = 1;
  time_units_spent = 0;

  actions.clear();

  for (std::vector<Troop*> player : troops)
  {
    for (auto* troop : player)
    {
      delete troop;
      troop = nullptr;
    }
    player.clear();
  }

  unit_count = 0;

  currency           = 100;
  shop_unit_selected = TroopTypes::NONE;
  units_bought_this_turn.clear();

  map.resetMap();
}
