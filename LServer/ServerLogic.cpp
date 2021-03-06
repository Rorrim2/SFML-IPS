#include "ServerLogic.h"



ServerLogic::ServerLogic(bool _windowEnable)
   :windowEnable(_windowEnable), playersManager(this->world), 
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
      p << itr.first << state.coords.x << state.coords.y << state.angle << state.health;
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
      std::function<void(void)> reqHandler = std::bind(&Server::requestHandling, &server);
      sf::Thread reqHandlerThread(reqHandler);
      reqHandlerThread.launch();
      this->commandThread.launch();
      initPsyhicsWorld();

      if (this->windowEnable == true)
      {
         initDebugDraw(this->window);
      }

      sf::Clock clock;
      sf::Time elapsedTime;
      sf::Time timeSinceLastUpdate = sf::Time::Zero;
      sf::Time timeSinceLastUpdateSnapshot = sf::Time::Zero;
      const sf::Time timePerFrame = sf::seconds(1.f / 60.f);
      const sf::Time timePerSnapshot = sf::seconds(1.f / 30.f);
      sf::Packet p;

      while (this->server.isRunning() == true && (this->windowEnable == true ? this->window->isDone() == false : true))
      {
         elapsedTime = clock.restart();
         timeSinceLastUpdate += elapsedTime;
         timeSinceLastUpdateSnapshot += elapsedTime;
         
         //std::cout << server.getTime().asSeconds() << std::endl;

         while (timeSinceLastUpdateSnapshot > timePerSnapshot)
         {
            {
               sf::Lock lock(server.getMutex());
               p = getPlayersSnapshot();
            }
            server.broadcast(p);
            p.clear();
            timeSinceLastUpdateSnapshot -= timePerFrame;
         }

         while (timeSinceLastUpdate > timePerFrame)
         {
            timeSinceLastUpdate -= timePerFrame;
            if (this->windowEnable == true)
            {
               this->window->update();
            }

            updatePsyhicsWorld();
         }
         
         if (this->windowEnable == true)
         {
            this->window->beginDraw();
            debugDraw();
            this->window->endDraw();
         }

         clearBodies();
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