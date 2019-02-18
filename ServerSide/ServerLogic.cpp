#include "ServerLogic.h"
#include <fstream>
#include <math.h>
#define PI 3.141592

ServerLogic::ServerLogic(bool _windowEnable)
   :windowEnable(_windowEnable), playersManager(world, this->server.getMutex()),
   server(&ServerLogic::handler, this), commandThread(std::bind(&ServerLogic::commandHandler, this, &this->server)),
   cannonballMg(world, this->server.getMutex())
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

   sf::Lock lock(server.getMutex());
   p << this->playersManager.getAllPlayers().size();
   for (auto &itr : this->playersManager.getAllPlayers())
   {
      PlayerState state = itr.second->getPlayerState();
      p << this->server.getTime().asMilliseconds() << itr.first << state.coords.x << state.coords.y << state.angle << state.health <<
         itr.second->getBody()->GetLinearVelocity().x << itr.second->getBody()->GetLinearVelocity().y << itr.second->getBody()->GetAngularVelocity() << (short)itr.second->getShipType();
   }
   return p;
}
sf::Packet ServerLogic::getCannonBallsSnapshot()
{
   sf::Packet p;
   StampPacket(PacketType::CannoBallUpdate, p);

   sf::Lock lock(server.getMutex());
   p << this->cannonballMg.getAllCannoballs().size();
   for (auto &itr : this->cannonballMg.getAllCannoballs())
   {
      p << this->server.getTime().asMilliseconds() << itr.first << itr.second->getPosition().x << itr.second->getPosition().y << itr.second->getBody()->GetLinearVelocity().x << itr.second->getBody()->GetLinearVelocity().y;
   }
   return p;
}
void ServerLogic::addPlayer(ClientID & clientID, const float & x, const float & y, ShipType shipType)
{
   this->playersManager.addPlayer(clientID, x, y, shipType);
}

void ServerLogic::movePlayer(ClientID & clientID, MoveDirection dir, const sf::Int32 & time)
{
   this->playersManager.movePlayer(clientID, dir, time);
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
         sf::Int32 time;
         //std::cout << id << "   " << count << std::endl;
         //sf::Lock lock(server->getMutex());
         sf::Lock lockCreate(bodyCreate);
         //std::ofstream plik1("plik1.txt", std::ofstream::out | std::ofstream::app);
         for (int i = 0; i < count; ++i)
         {
            packet >> time >> dir;

            ServerPlayer *player = this->playersManager.getPlayer(id);

            if (player->canShoot())
            {
               sf::Vector2f position = player->getPosition(); //pozycja
               float angle = player->getBody()->GetAngle();

               float x = std::cos(player->getBody()->GetAngle()); //dlaczego nie x/z?
               float y = std::sin(player->getBody()->GetAngle());//dlaczego nie y/z?

               if (dir & MoveDirection::SHOOT_RIGHT)
               {
                  auto rot = player->getBody()->GetTransform().q;
                  float temp_angle = fabs(angle);
                  while (temp_angle > 2 * PI)
                  {
                     temp_angle -= 2 * PI;
                  }
                  float new_x, new_y, temp_x, temp_y;
                  temp_x = sin(temp_angle) * player->width * METERS_PER_PIXEL;
                  temp_y = cos(temp_angle) * player->width * METERS_PER_PIXEL;
                  new_x = position.x + temp_x;
                  new_y = position.y + temp_y;
                  ServerCannonball *cannonBall = this->cannonballMg.addCannonball(PIXELS_PER_METER * new_x, PIXELS_PER_METER * new_y, player);
                  auto velocityVector = player->getBody()->GetLinearVelocity();
                  //cannonBall->getBody()->ApplyForceToCenter(b2Vec2(1 * rot.GetYAxis().x / 2 + 1 * velocityVector.x / 4, 1 * rot.GetXAxis().x / 2 + 1 * velocityVector.y / 4), true);
                  cannonBall->getBody()->ApplyForceToCenter(b2Vec2(1 * rot.GetYAxis().x / 2, 1 * rot.GetXAxis().x / 2), true);
                  player->shoot();
                  DEBUG_COUT("ship " << player->getBody()->GetAngle() << " is shooting right  " << rot.GetYAxis().x << "  " << rot.GetXAxis().x << "      " << rot.GetXAxis().y);
                  //plik1 << "ship " << player->getBody()->GetAngle() << " is shooting right  " << rot.GetXAxis().x << "  " << x << "  " << rot.GetXAxis().y << "      " << rot.GetYAxis().x << "  " << x << "  " << rot.GetYAxis().y << endl;
               }

               if (dir & MoveDirection::SHOOT_LEFT)
               {
                  auto rot = player->getBody()->GetTransform().q;
                  float temp_angle = fabs(angle);
                  while (temp_angle > 2 * PI)
                  {
                     temp_angle -= 2 * PI;
                  }
                  float new_x, new_y, temp_x, temp_y;
                  temp_x = sin(temp_angle) * player->width * METERS_PER_PIXEL;
                  temp_y = cos(temp_angle) * player->width * METERS_PER_PIXEL;
                  new_x = position.x - temp_x;
                  new_y = position.y - temp_y;
                  ServerCannonball *cannonBall = this->cannonballMg.addCannonball(PIXELS_PER_METER * new_x, PIXELS_PER_METER * new_y, player);
                  auto velocityVector = player->getBody()->GetLinearVelocity();
                  //cannonBall->getBody()->ApplyForceToCenter(b2Vec2(1 * rot.GetYAxis().x / 2 + 1 * velocityVector.x / 4, 1 * rot.GetXAxis().x / 2 + 1 * velocityVector.y / 4), true);
                  //cannonBall->getBody()->ApplyForceToCenter(b2Vec2(1 * rot.GetYAxis().x / 2, 1 * rot.GetYAxis().y / 2), true);
                  cannonBall->getBody()->ApplyForceToCenter(b2Vec2(-1 * rot.GetYAxis().x / 2, -1 * rot.GetXAxis().x / 2), true);
                  player->shoot();
                  DEBUG_COUT("ship " << player->getBody()->GetAngle() << " is shooting right  " << rot.GetYAxis().x << "  " << rot.GetYAxis().y << "      " << rot.GetYAxis().x);
                  //plik1 << "ship " << player->getBody()->GetAngle() << " is shooting right  " << rot.GetXAxis().x << "  " << x << "  " << rot.GetXAxis().y << "      " << rot.GetYAxis().x << "  " << x << "  " << rot.GetYAxis().y << endl;

               }
            }
            // DEBUG_COUT((int)dir << "server: " << this->server.getTime().asMilliseconds() << " time: " << time << " diff time"  << this->server.getTime().asMilliseconds() - time);
            movePlayer(id, static_cast<MoveDirection>(dir), this->server.getTime().asMilliseconds() - time);
         }
         //plik1.close();
      }
      else if (static_cast<PacketType>(packetID) == PacketType::PlayerCreate)
      {
         //TODO this x and y should get from map
         float x = 80, y = 400;
         ClientID clientID;
         short shipType;
         packet >> clientID >> shipType;
         sf::Packet newPacket;
         StampPacket(PacketType::PlayerCreate, newPacket);
         newPacket << clientID << x << y << shipType;
         DEBUG_COUT("Creating ship");
         {
            sf::Lock lock(this->server.getMutex());
            addPlayer(clientID, x, y, (ShipType)shipType);
         }
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
      this->world.loadMap("map.xml");
      this->world.addPolygons();
      if (this->windowEnable == true)
      {
         initDebugDraw(this->window);
      }

      const sf::Time timePerSnapshot = sf::seconds(1.f / 15.f);
      sf::Time timeSinceLastUpdateSnapshot = sf::Time::Zero;

      sf::Time elapsedTime;
      sf::Time color = sf::Time::Zero;
      sf::Time timeSinceLastUpdate = sf::Time::Zero;
      const sf::Time timePerFrame = sf::seconds(1.f / 30.f);
      sf::Packet p;


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
            p = getPlayersSnapshot();
            server.broadcast(p);
            p.clear();
            p = getCannonBallsSnapshot();
            server.broadcast(p);
            p.clear();
            timeSinceLastUpdateSnapshot -= timePerSnapshot;
            this->server.update(timePerFrame);
            timeSinceLastUpdate -= timePerFrame;
            if (this->windowEnable == true)
            {
               this->window->update();
            }

            this->playersManager.update(timePerFrame);
            this->cannonballMg.update(timePerFrame);
            {
               sf::Lock lock(bodyCreate);
               updatePsyhicsWorld();
            }
            for (auto & player : this->playersManager.getAllPlayers())
            {
               if (player.second->isDead() == true)
               {
                  sf::Packet p;
                  StampPacket(PacketType::GameOver, p);
                  p << player.first;
                  sf::Lock lock(server.getMutex());
                  server.send(player.first, p);
               }
            }

            this->world.eraseDeathBodies();
         }

         if (this->windowEnable == true)
         {
            this->window->beginDraw();
            debugDraw();
            this->window->endDraw();
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
