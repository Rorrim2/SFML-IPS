#include "ClientPlayersManager.h"
#include "Global.h"


ClientPlayersManager::ClientPlayersManager(World &_world)
   : world(_world)
{
   this->players.clear();
}


ClientPlayersManager::~ClientPlayersManager()
{
   for (auto &iter : this->players)
   {
      DELLISNOTNULL(iter.second);
   }

   this->players.clear();
}

void ClientPlayersManager::addPlayer(const ClientID & clientID, const float & x, const float & y)
{
   if (this->players.count(clientID) <= 0)
   {
      auto iter = std::find_if(this->playersToCreate.begin(), this->playersToCreate.end(), [clientID](auto tuple)->bool {return clientID == std::get<0>(tuple); });
      if(iter == this->playersToCreate.end())
      {
         this->playersToCreate.push_back(std::make_tuple(clientID, x, y));
      }
   }
}
void ClientPlayersManager::addPlayer(const ClientID & clientID, ClientPlayer *player)
{
   this->players[clientID] = player;
}
void ClientPlayersManager::movePlayer(const ClientID & clientID, const float & x, const float & y, const float & angle)
{
   if (this->players.count(clientID) <= 0)
   {
      addPlayer(clientID, x, y);
   }
   else
   {
      /*player->setPosition(sf::Vector2f(x, y));
      player->setAngle(angle);*/
      this->lastUpdates[clientID] = 5;
   }
}

void ClientPlayersManager::removePlayer(const ClientID & clientID)
{
   if (this->players.count(clientID) > 0)
   {
      this->world.removeBody(this->players[clientID]->getBody());
      DELLISNOTNULL(this->players[clientID]);
      this->players.erase(clientID);
   }
}

ClientPlayer* ClientPlayersManager::getPlayer(const ClientID & clientID)
{
   return this->players[clientID];
}

void ClientPlayersManager::drawAllPlayers(Window & window)
{
   for (auto &iter : this->players)
   {
      iter.second->draw(window);
   }
}

void ClientPlayersManager::updateAllPlayers(const sf::Time & time)
{
   for (auto &iter : this->players)
   {
      iter.second->update(time);
   }
   //find and erase players sprites that they not updated from snapshots
   //for (auto iter = this->lastUpdates.cbegin(), next_it = iter; iter != this->lastUpdates.cend(); iter = next_it)
   //{
   //   ++next_it;
   //   if(iter->second <= 0)
   //   {
   //      removePlayer(iter->first);
   //      this->lastUpdates.erase(iter->first);
   //   }
   //}

}

void ClientPlayersManager::decreasePlayerOccurence()
{
   for (auto &iter : this->lastUpdates)
   {
      iter.second--;
   }
}

void ClientPlayersManager::createShips()
{
   for (auto iter : this->playersToCreate)
   {
      this->players[std::get<0>(iter)] = new ClientPlayer(createShipBody(std::get<1>(iter), std::get<2>(iter)));
      this->lastUpdates[std::get<0>(iter)] = 5;
   }
   this->playersToCreate.clear();
}

b2Body * ClientPlayersManager::createShipBody(float x, float y)
{
   b2Body *body = this->world.createBody(x, y);
   this->world.createBoxFixture(body, 25, 25, 1, 0.5f);
   return body;
}
