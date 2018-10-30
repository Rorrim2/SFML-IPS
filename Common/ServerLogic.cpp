#include "ServerLogic.h"



ServerLogic::ServerLogic()
   : playersManager(this->world)
{
}


ServerLogic::~ServerLogic()
{
}

void ServerLogic::initPsyhicsWorld()
{
   this->world.initWorld();
}

void ServerLogic::updatePsyhicsWorld()
{
   this->world.updateWorld();
}

std::vector<sf::Packet> ServerLogic::getPlayersPos()
{
   std::vector<sf::Packet> packets;
   for (auto &itr : this->playersManager.getAllPlayers())
   {
      sf::Packet p;
      StampPacket(PacketType::PlayerUpdate, p);
      p << itr.first << itr.second->getPosition().x << itr.second->getPosition().y << itr.second->getAngle();
      packets.push_back(p);
   }
   return packets;
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
