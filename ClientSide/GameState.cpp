#include "GameState.h"

GameState::GameState(StateManager *stateManager)
   :BaseState(stateManager), playersManager(world, this->client.getMutex()), cannonBallManager(world, this->client.getMutex())
{
   this->physicStarted = false;
   map.setMapName("map.png");
   map.loadFromFile();
   this->world.loadMap("map.xml");
   this->textureManager.RequireResource("HPBar1");
   this->textureManager.RequireResource("HPBar2");
   this->textureManager.RequireResource("HPBar3");
   this->hpBar = new sf::Sprite(*this->textureManager.GetResource("HPBar1"));
}


GameState::~GameState()
{
}

void GameState::onCreate()
{
   sf::IpAddress ip("localhost");
   PortNumber port = 5600;
   //std::cout << "Enter Server IP: ";
   //std::cin >> ip;
   //std::cout << "Enter Server Port: ";
   //std::cin >> port;
   //sf::sleep(sf::seconds(10));
   setServer(ip, port);
   this->client.setup(&GameState::clientHandler, this);

   //event manager 
   EventManager* evMgr = this->stateManager->getContext()->eventManager;

   evMgr->AddCallback(StateTypeE::GAME, "KeyEscape", &GameState::moveToMainMenu, this);
   evMgr->AddCallback(StateTypeE::GAME, "Shoot_Left", &GameState::shoot, this);
   evMgr->AddCallback(StateTypeE::GAME, "Shoot_Right", &GameState::shoot, this);

   //wait for synchronize time with server
   if (this->client.connect() == false)
   {
      moveToMainMenu(nullptr);
   }
}

void GameState::onDestroy()
{
   DELLISNOTNULL(this->player);
   EventManager* evMgr = this->stateManager->getContext()->eventManager;
   evMgr->RemoveCallback(StateTypeE::GAME, "KeyEscape");
   evMgr->RemoveCallback(StateTypeE::GAME, "Shoot_Left");
   evMgr->RemoveCallback(StateTypeE::GAME, "Shoot_Right");

   //TODO add destroying world
}

void GameState::draw()
{
   this->stateManager->getContext()->window->draw(this->map.getMapSprite());
   if (this->physicStarted == true)
   {
      this->playersManager.drawAllPlayers(*this->stateManager->getContext()->window);
      this->cannonBallManager.drawAllCannonbalss(*this->stateManager->getContext()->window);
      if (this->debug)
      {
         this->world.drawDebugData();
      }
      this->stateManager->getContext()->window->draw(*this->hpBar);
   }
}

void GameState::update(const sf::Time & time)
{
   //before sync timer
   client.update(time);
   this->sendEventToServerTimer += time.asMilliseconds();
   if (this->client.isSynced() == false) return;

   if (this->physicStarted == false)
   {
      this->world.initWorld();
      this->world.addPolygons();
      this->world.initDebugDrawing(*this->stateManager->getContext()->window);

      this->client.sendCreatePlayerPacket(this->stateManager->getContext()->window->GetShipType());
      this->player = this->playersManager.createPlayer(80, 400, this->stateManager->getContext()->window->GetShipType());
      //new ClientPlayer(this->playersManager.createShipBody(400, 400), *this->stateManager->getContext()->textureManager->GetResource("Ship_red"));
      this->playersManager.addPlayer(this->client.getClientID(), this->player);
      this->physicStarted = true;
   }
   if (this->player->getHealth() != this->hp)
   {
      if (this->player->getHealth() == 2)
      {
         this->hpBar->setTexture(*this->textureManager.GetResource("HPBar2"));
      }
      else
      {
         this->hpBar->setTexture(*this->textureManager.GetResource("HPBar3"));
      }

      this->hp = this->player->getHealth();
   }
   sf::Time t = this->client.getTime();
   if (t - this->timer >= sf::seconds(1))
   {
      std::cout << t.asSeconds() << std::endl;
      this->timer = t;
   }

   {
      sf::Lock lock(this->client.getMutex());
      this->world.updateWorld();
      this->playersManager.updateAllPlayers(time);
      this->cannonBallManager.updateAllCannonballs(time);
      movePlayer(nullptr);

      this->world.eraseDeathBodies();
   }

   if (this->sendEventToServerTimer >= 10)
   {
      if (this->lastDirections.size() > 0)
      {
         sf::Packet p;
         StampPacket(PacketType::PlayerMove, p);
         p << this->lastDirections.size();
         for (int i = 0; i < this->lastDirections.size(); ++i)
         {
            p << this->lastDirections.front().second << this->lastDirections.front().first;
            this->lastDirections.pop();
         }
         this->client.sendPacket(p);
      }
      this->sendEventToServerTimer = 0;
   }

   if (this->client.isConnected() == false)
   {
      moveToMainMenu(nullptr);
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
   DEBUG_COUT("Deactive");

   this->client.disconnect();
}

void GameState::setServer(const sf::IpAddress & ip, const PortNumber & portNumber)
{
   this->client.setServer(ip, portNumber);
}

bool GameState::connect()
{
   return this->client.connect();
}

void GameState::moveToMainMenu(EventDetails *details)
{
   this->stateManager->switchTo(StateTypeE::MENU);
}



void GameState::shoot(EventDetails * details)
{
   if (this->player->canShoot() == true)
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
      this->lastDirections.push({ dir, this->client.getTime().asMilliseconds() });
   }
}

void GameState::movePlayer(EventDetails *details)
{
   MoveDirection dir = MoveDirection::NONE;
   if (this->stateManager->getContext()->window->getFocus() == true)
   {
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
      {
         this->lastDirections.push({ MoveDirection::LEFT, this->client.getTime().asMilliseconds() });
         this->player->move(MoveDirection::RIGHT);
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
      {
         this->lastDirections.push({ MoveDirection::RIGHT, this->client.getTime().asMilliseconds() });
         this->player->move(MoveDirection::LEFT);
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
      {
         this->lastDirections.push({ MoveDirection::FORWARD, this->client.getTime().asMilliseconds() });
         //this->player->move(MoveDirection::FORWARD);
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
      {
         this->lastDirections.push({ MoveDirection::BACKWARD, this->client.getTime().asMilliseconds() });
         //this->player->move(MoveDirection::BACKWARD);
      }

      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::T))
      {
         this->debug = !this->debug;
      }
   }
}


void GameState::clientHandler(const PacketID &id, sf::Packet &packet, Client *client)
{
   if (this->physicStarted == true)
   {
      if (static_cast<PacketType>(id) == PacketType::Message)
      {
         std::string message;
         packet >> message;
         std::cout << message << std::endl;
      }
      //else if (static_cast<PacketType>(id) == PacketType::PlayerCreate)
      //{
      //   float x, y;
      //   ClientID idC;
      //   packet >> idC >> x >> y;
      //   playersManager.addPlayer(idC, x, y);
      //}
      else if (static_cast<PacketType>(id) == PacketType::PlayerUpdate)
      {
         playersManager.decreasePlayerOccurence();
         size_t count;
         short health;
         int time;
         float x, y, angle, angularVel;
         ClientID idC;
         b2Vec2 linearVelocity;
         short shipType;
         packet >> count;

         for (int i = 0; i < count; ++i)
         {
            packet >> time >> idC >> x >> y >> angle >> health >> linearVelocity.x >> linearVelocity.y >> angularVel >> shipType;
            //if (std::fabs(this->client.getTime().asMilliseconds() - time) < 150)
            {
               playersManager.movePlayer({ this->client.getTime().asMilliseconds() - time, idC, x, y, angle, angularVel, linearVelocity, (ShipType)shipType });
               playersManager.updateHealth(idC, health);
            }
         }
      }
      else if (static_cast<PacketType>(id) == PacketType::CannoBallUpdate)
      {
         cannonBallManager.decreaseCannonballOccurence();
         size_t count;
         sf::Int32 time;
         float x, y;
         CannID idC;
         b2Vec2 linearVelocity;
         packet >> count;

         for (int i = 0; i < count; ++i)
         {
            packet >> time >> idC >> x >> y >> linearVelocity.x >> linearVelocity.y;
            if (std::fabs(this->client.getTime().asMilliseconds() - time) < 150)
            {
               std::cerr << idC << std::endl;
               cannonBallManager.moveCannonball({ this->client.getTime().asMilliseconds() - time, idC, x, y, linearVelocity });
            }
         }
      }
      else if (static_cast<PacketType>(id) == PacketType::GameOver)
      {
         client->disconnect();
         DEBUG_COUT("Disconnected!");
         this->stateManager->switchTo(StateTypeE::GAME_OVER);
      }
      else if (static_cast<PacketType>(id) == PacketType::Disconnect)
      {
         client->disconnect();
         DEBUG_COUT("Disconnected!");
         moveToMainMenu(nullptr);
      }
   }
}
