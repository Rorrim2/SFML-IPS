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
