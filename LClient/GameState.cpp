#include "GameState.h"

void clientHandler(const PacketID &id, sf::Packet &packet, Client *client);

GameState::GameState(StateManager *stateManager)
   :BaseState(stateManager), playersManager(world), connection(std::bind(&GameState::updateConnection, this, &this->client))
{
   this->world.initWorld();
   //this->world.initDebugDrawing(*this->stateManager->getContext()->window);
}


GameState::~GameState()
{
}

void GameState::onCreate()
{
   sf::IpAddress ip("localhost");
   PortNumber port = 5600;
   std::cout << "Enter Server IP: ";
   std::cin >> ip;
   std::cout << "Enter Server Port: ";
   std::cin >> port;
   setServer(ip, port);
   this->client.setup(&GameState::clientHandler, this);

   //event manager 
   //TODO ask if this is important and how to change it
   EventManager* evMgr = this->stateManager->getContext()->eventManager;

   evMgr->AddCallback(StateTypeE::GAME, "KeyEscape", &GameState::moveToMainMenu, this);
   //evMgr->AddCallback(StateTypeE::GAME, "Ship_Move_Up", &GameState::movePlayer, this);
   //evMgr->AddCallback(StateTypeE::GAME, "Ship_Move_Down", &GameState::movePlayer, this);
   //evMgr->AddCallback(StateTypeE::GAME, "Ship_Move_Left", &GameState::movePlayer, this);
   //evMgr->AddCallback(StateTypeE::GAME, "Ship_Move_Right", &GameState::movePlayer, this);

   evMgr->AddCallback(StateTypeE::GAME, "Shoot_Left", &GameState::shoot, this);
   evMgr->AddCallback(StateTypeE::GAME, "Shoot_Right", &GameState::shoot, this);
   
   connect();
   if (this->client.isConnected() == false)
   {
      moveToMainMenu(nullptr);
   }
   sf::sleep(sf::seconds(1));
   this->timer = this->client.getTime();
}

void GameState::onDestroy()
{
   DELLISNOTNULL(this->player);
   this->client.disconnect();
   EventManager* evMgr = this->stateManager->getContext()->eventManager;
   evMgr->RemoveCallback(StateTypeE::GAME, "KeyEscape");
   //evMgr->RemoveCallback(StateTypeE::GAME, "Ship_Move_Up");
   //evMgr->RemoveCallback(StateTypeE::GAME, "Ship_Move_Down");
   //evMgr->RemoveCallback(StateTypeE::GAME, "Ship_Move_Left");
   //evMgr->RemoveCallback(StateTypeE::GAME, "Ship_Move_Right");
   evMgr->RemoveCallback(StateTypeE::GAME, "Shoot_Left");
   evMgr->RemoveCallback(StateTypeE::GAME, "Shoot_Right");
}

void GameState::draw()
{
   //this->world.drawDebugData();
   this->playersManager.drawAllPlayers(*this->stateManager->getContext()->window);
}
bool r = false;
void GameState::update(const sf::Time & time)
{
   if (this->client.isSynced() == false || this->client.getTime() < sf::seconds(60)) return;

   movePlayer(nullptr);

   //client.update(time);

   this->world.updateWorld();
   this->playersManager.updateAllPlayers(time);

   sf::Time cl = this->client.getTime();
   if (cl - this->timer >= sf::seconds(1))
   {
      //this->timer = sf::seconds(std::floor(this->client.getTime().asSeconds()));
      this->timer = cl;
      if (r)
      {
         this->player->sprite->setFillColor(sf::Color::Green);
      }
      else
      {
         this->player->sprite->setFillColor(sf::Color::Red);
      }
      r = !r;

      std::cout << "    " << cl.asMicroseconds() << "    " << cl.asSeconds() << "    " << client.getServerTime().asSeconds() << std::endl;
   }




  // std::cout<< this->gameTimer  << "  " << this->client.getTime().asSeconds() << "     " << time.asSeconds() << "  " << this->client.getLastTimeHeartBeat().asSeconds() << std::endl;
   this->sendEventToServerTimer += time.asMilliseconds();
   //std::cout << this->client.getTime().asSeconds() << "     " << time.asSeconds() << std::endl;
   if (this->sendEventToServerTimer >= 10)
   {
      if (this->lastDirections.size() > 0)
      {
         sf::Packet p;
         StampPacket(PacketType::PlayerMove, p);
         p << this->lastDirections.size();
         for (int i = 0; i < this->lastDirections.size(); ++i)
         {
            p << this->lastDirections.front();
            this->lastDirections.pop();
         }
         this->client.sendPacket(p);
      }
      this->sendEventToServerTimer = 0;
   }

}

void GameState::activate()
{
   this->sendEventToServerTimer = 0.0f;

   if (this->client.isConnected() == false)
   {
      if (connect() == false)
      {
         moveToMainMenu(nullptr);
      }
   }
}

void GameState::deactivate()
{
   //this->client.disconnect();
}

void GameState::setServer(const sf::IpAddress & ip, const PortNumber & portNumber)
{
   this->client.setServer(ip, portNumber);
}

void GameState::updateConnection(Client *client)
{
   sf::Clock clock;
   clock.restart();
   while (client->isConnected())
   {
      client->update(clock.restart());
   }
   moveToMainMenu(nullptr);
}

bool GameState::connect()
{
   //sf::Thread connection(std::bind(&GameState::updateConnection, this, &this->client));
   bool rV = this->client.connect();
   if (rV == true)
   {
      this->client.sendCreatePlayerPacket();
      this->player = new ClientPlayer(this->playersManager.createShipBody(400, 400));
      this->playersManager.addPlayer(this->client.getClientID(), this->player);
      connection.launch();
   }
   return rV;
}

void GameState::moveToMainMenu(EventDetails *details)
{
   this->stateManager->switchTo(StateTypeE::MENU);
}



void GameState::shoot(EventDetails * details)
{
   MoveDirection dir = MoveDirection::NONE;
   switch (details->keyCode)
   {
   case sf::Keyboard::Key::F:
   {
      dir = MoveDirection::SHOOT_LEFT;
      break;
   }
   case sf::Keyboard::Key::E:
   {
      dir = MoveDirection::SHOOT_RIGHT;
      break;
   }
   }
   this->lastDirections.push(dir);
}

void GameState::movePlayer(EventDetails *details)
{
   MoveDirection dir = MoveDirection::NONE;
   if (this->stateManager->getContext()->window->getFocus() == true)
   {
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
      {
         this->lastDirections.push(MoveDirection::LEFT);
         this->player->move(MoveDirection::LEFT);
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
      {
         this->lastDirections.push(MoveDirection::RIGHT);
         this->player->move(MoveDirection::RIGHT);
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
      {
         this->lastDirections.push(MoveDirection::FORWARD);
         this->player->move(MoveDirection::FORWARD);
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
      {
         this->lastDirections.push(MoveDirection::BACKWARD);
         this->player->move(MoveDirection::BACKWARD);
      }
   }
   //switch (details->keyCode)
   //{
   //case sf::Keyboard::Key::A:
   //{
   //   dir = MoveDirection::LEFT;
   //   break;
   //}
   //case sf::Keyboard::Key::D:
   //{
   //   dir = MoveDirection::RIGHT;
   //   break;
   //}
   //case sf::Keyboard::Key::W:
   //{
   //   dir = MoveDirection::FORWARD;
   //   break;
   //}
   //case sf::Keyboard::Key::S:
   //{
   //   dir = MoveDirection::BACKWARD;
   //   break;
   //}
   //}
//   this->lastDirections.push(dir);
}


void GameState::clientHandler(const PacketID &id, sf::Packet &packet, Client *client)
{
   if (static_cast<PacketType>(id) == PacketType::Message)
   {
      std::string message;
      packet >> message;
      std::cout << message << std::endl;
   }
   else if (static_cast<PacketType>(id) == PacketType::PlayerCreate)
   {
      float x, y;
      ClientID idC;
      packet >> idC >> x >> y;
      playersManager.addPlayer(idC, x, y);
   }
   else if (static_cast<PacketType>(id) == PacketType::PlayerUpdate)
   {
      playersManager.decreasePlayerOccurence();
      size_t count;
      int health;
      float x, y, angle;
      ClientID idC;
      packet >> count;
      for (int i = 0; i < count; ++i)
      {
         packet >> idC >> x >> y >> angle >> health;
         playersManager.movePlayer(idC, x, y, angle);
      }
   }
   else if (static_cast<PacketType>(id) == PacketType::Disconnect)
   {
      client->disconnect();
      DEBUG_COUT("Disconnected!");
      moveToMainMenu(nullptr);
   }
}
