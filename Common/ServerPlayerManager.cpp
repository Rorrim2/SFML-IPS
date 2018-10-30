#include "ServerPlayerManager.h"
#include "Global.h"

ServerPlayersManager::ServerPlayersManager(World &world)
   : box2DWorld(world)
{
}


ServerPlayersManager::~ServerPlayersManager()
{
   for (auto &iter : this->players)
   {
      DELLISNOTNULL(iter.second);
   }

   this->players.clear();
}

void ServerPlayersManager::addPlayer(const ClientID & clientID, const float & x, const float & y)
{
   this->players.emplace(clientID, new ServerPlayer(&this->box2DWorld, x, y));
}

void ServerPlayersManager::removePlayer(const ClientID & clientID)
{
   this->box2DWorld.removeBody(this->players[clientID]->getBody());
   DELLISNOTNULL(this->players[clientID]);
   this->players.erase(clientID);
}

ServerPlayer* ServerPlayersManager::getPlayer(const ClientID & clientID)
{
   return this->players[clientID];
}

ServerPlayers& ServerPlayersManager::getAllPlayers()
{
   return this->players;
}


//TODO change this speed to not hardcoded
void ServerPlayersManager::movePlayer(const ClientID & id, MoveDirection dir)
{
   this->players[id]->move(5.f, dir);
}
