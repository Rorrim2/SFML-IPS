#include "ServerPlayerManager.h"
#include "Global.h"

ServerPlayersManager::ServerPlayersManager(World &world, sf::Mutex &_mutex)
   : world(world), mutex(_mutex)
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
   if (this->players.count(clientID) <= 0)
   {
      sf::Lock lock(this->mutex);
      this->players[clientID] = new ServerPlayer(createShipBody(x, y));
   }
}

void ServerPlayersManager::removePlayer(const ClientID & clientID)
{
   if (this->players.find(clientID) != this->players.end())
   {
      this->world.removeBody(this->players[clientID]->getBody());
      DELLISNOTNULL(this->players[clientID]);
      this->players.erase(clientID);
   }
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
   this->players[id]->move(dir);
}

void ServerPlayersManager::removeAllPlayers()
{
   for (int i = 0; i < this->players.size(); i++)
   {
      this->world.removeBody(this->players[i]->getBody());
      DELLISNOTNULL(this->players[i]);
      this->players.erase(i);
   }
}

b2Body *ServerPlayersManager::createShipBody(float x, float y)
{
   b2Body *body = this->world.createBody(x, y);
   this->world.createBoxFixture(body, 25, 25, 1, 0.5f);
   return body;
}
