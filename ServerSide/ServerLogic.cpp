#include "ServerLogic.h"



ServerLogic::ServerLogic(bool _windowEnable)
   :windowEnable(_windowEnable), playersManager(world, this->server.getMutex()),
   server(&ServerLogic::handler, this), commandThread(std::bind(&ServerLogic::commandHandler, this, &this->server))
{
   this->server.bindTimeoutHandler(&ServerLogic::clientLeft, this);
   if (this->windowEnable == true)
   {
      this->window = new Window("Server", sf::Vector2u(900, 800));
   }
}


ServerLogic::~ServerLogic()
{
   DELLISNOTNULL(this->window);
}

void ServerLogic::addObjects()
{
	world.initMap();
	world.addRectangles();
	world.addPolygons();
	/*b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;

	b2World* temp_world = world.getWorld();

	b2Body* body = temp_world->CreateBody(&bodyDef);

	b2Vec2 vs[4];
	vs[0].Set(0.0f, 0.0f);
	vs[1].Set(10.0f, 0.0f);
	vs[2].Set(10.0f, 10.0f);
	vs[3].Set(0.0f, 10.0f);

	b2ChainShape chain;
	chain.CreateLoop(vs, 4);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &chain;
	fixtureDef.density = 1;
	fixtureDef.friction = 1;
	fixtureDef.restitution = 0.5f;
	body->CreateFixture(&fixtureDef);*/

}

void ServerLogic::initPsyhicsWorld()
{
   this->world.initWorld();
}

void ServerLogic::updatePsyhicsWorld()
{
   this->world.updateWorld();
}

sf::Packet ServerLogic::getPlayersSnapshot()
{
   sf::Packet p;
   StampPacket(PacketType::PlayerUpdate, p);
   p << this->playersManager.getAllPlayers().size();
   for (auto &itr : this->playersManager.getAllPlayers())
   {
      PlayerState state = itr.second->getPlayerState();
      p << itr.first << state.coords.x << state.coords.y << state.angle << state.health << itr.second->getBody()->GetLinearVelocity().x << itr.second->getBody()->GetLinearVelocity().y;
   }
   return p;
}

void ServerLogic::addPlayer(ClientID & clientID, const float & x, const float & y)
{
   this->playersManager.addPlayer(clientID, x, y);
}

void ServerLogic::movePlayer(ClientID & clientID, MoveDirection dir)
{
   this->playersManager.movePlayer(clientID, dir);
}

void ServerLogic::signToRemovePlayer(const ClientID & clientID)
{
   this->playersManager.removePlayer(clientID);
}

void ServerLogic::initDebugDraw(Window *window)
{
   if (window != nullptr)
   {
      this->world.initDebugDrawing(*window);
   }
}
void ServerLogic::debugDraw()
{
   this->world.drawDebugData();
}
void ServerLogic::clearBodies()
{
   this->world.eraseDeathBodies();
}

void ServerLogic::clientLeft(const ClientID & clientID)
{
   sf::Lock lock(this->server.getMutex());
   signToRemovePlayer(clientID);
}

void ServerLogic::handler(sf::IpAddress &ip, const PortNumber &port, const PacketID &packetID, sf::Packet &packet, Server *server)
{
   ClientID id = server->getClientID(ip, port);

   if (id != static_cast<ClientID>(Network::NullID))
   {
      if (static_cast<PacketType>(packetID) == PacketType::Disconnect)
      {
         sf::Packet newPacket;
         StampPacket(PacketType::Disconnect, newPacket);
         signToRemovePlayer(id);
      }
      else if (static_cast<PacketType>(packetID) == PacketType::Message)
      {
         std::string receiveMsg;
         packet >> receiveMsg;

         std::string msg = ip.toString() + ": " + std::to_string(port) + " :" + receiveMsg;
         DEBUG_COUT(msg);
         sf::Packet newPacket;
         StampPacket(PacketType::Message, newPacket);
         newPacket << msg;

         server->broadcast(newPacket, id);
      }
      else if (static_cast<PacketType>(packetID) == PacketType::PlayerMove)
      {
         size_t count;
         int dir;
         packet >> count;
         //std::cout << id << "   " << count << std::endl;
         sf::Lock lock(server->getMutex());
         for (int i = 0; i < count; ++i)
         {
            packet >> dir;

            //TODO �adnie to gdzie� wywalic
            if (dir == MoveDirection::SHOOT_LEFT)
            {
               DEBUG_COUT("ship " << id << " is shooting left");
            }
            if (dir == MoveDirection::SHOOT_RIGHT)
            {
               DEBUG_COUT("ship " << id << " is shooting RIGHT");
            }
            movePlayer(id, static_cast<MoveDirection>(dir));
         }
      }
      else if (static_cast<PacketType>(packetID) == PacketType::PlayerCreate)
      {
         //TODO this x and y should get from map
         float x = 400, y = 400;
         ClientID clientID;
         packet >> clientID;
         sf::Packet newPacket;
         StampPacket(PacketType::PlayerCreate, newPacket);
         newPacket << clientID << x << y;
         DEBUG_COUT("Creating ship");

         addPlayer(clientID, x, y);
         server->broadcast(newPacket, id);
      }
   }
   else if (static_cast<PacketType>(packetID) == PacketType::Connect)
   {
      ClientID id = server->addClient(ip, port);
      sf::Packet newPacket;
      StampPacket(PacketType::Connect, newPacket);
      newPacket << id;
      std::cout << server->send(id, newPacket) << "   " << ip << "  " << port << std::endl;
   }

}

void ServerLogic::runServer()
{
   if (server.start() == true)
   {
      this->commandThread.launch();
      initPsyhicsWorld();

      if (this->windowEnable == true)
      {
         initDebugDraw(this->window);
      }

      const sf::Time timePerSnapshot = sf::seconds(1.f / 20.f);
      sf::Time timeSinceLastUpdateSnapshot = sf::Time::Zero;

      sf::Time elapsedTime;
      sf::Time color = sf::Time::Zero;
      sf::Time timeSinceLastUpdate = sf::Time::Zero;
      const sf::Time timePerFrame = sf::seconds(1.f / 30.f);
      sf::Packet p;


	  addObjects();
	  //b2Body* cokolwiek = this->world.createBody(100, 100);
	  //this->world.createBoxFixture(cokolwiek, 20, 10);
	  world.debugBodie();

      sf::Clock clock;
      while (this->server.isRunning() == true && (this->windowEnable == true ? this->window->isDone() == false : true))
      {
         elapsedTime = clock.restart();
         timeSinceLastUpdate += elapsedTime;
         timeSinceLastUpdateSnapshot += elapsedTime;
         color += elapsedTime;
         
         //std::cout << server.getTime().asSeconds() << std::endl;
         if (color >= sf::seconds(1))
         {
            std::cout << this->server.getTime().asMicroseconds() << "   " << this->server.getTime().asSeconds() << std::endl;
            color = sf::Time::Zero;
         }

         clearBodies();
         while (timeSinceLastUpdate > timePerFrame)
         {
			sf::Lock lock(this->server.getMutex());
            this->server.update(timePerFrame);
            timeSinceLastUpdate -= timePerFrame;
            if (this->windowEnable == true)
            {
               this->window->update();
            }
            updatePsyhicsWorld();
            this->world.eraseDeathBodies();
         }
         
         if (this->windowEnable == true)
         {
            this->window->beginDraw();
            debugDraw();
            this->window->endDraw();
         }


         while (timeSinceLastUpdateSnapshot > timePerSnapshot)
         {
            {
               sf::Lock lock(server.getMutex());
               p = getPlayersSnapshot();
            }
            server.broadcast(p);
            p.clear();
            timeSinceLastUpdateSnapshot -= timePerSnapshot;
         }
      }
   }
}

void ServerLogic::commandHandler(Server *server)
{
   while (server->isRunning())
   {
      std::string str;
      std::getline(std::cin, str);
      if (str == "quit")
      {
         server->stop();
         break;
      }
      else if (str == "dc")
      {
         this->playersManager.removeAllPlayers();
         server->disconnectAll();
      }

   }
}