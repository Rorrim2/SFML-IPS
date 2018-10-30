#include "ClientPlayersManager.h"
#include <Global.h>


ClientPlayersManager::ClientPlayersManager()
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
   this->players[clientID] = new ClientPlayer(x, y);
}
void ClientPlayersManager::addPlayer(const ClientID & clientID, ClientPlayer *player)
{
   this->players[clientID] = player;
}
void ClientPlayersManager::movePlayer(const ClientID & clientID, const float & x, const float & y, const float & angle)
{
   ClientPlayer *player = this->players[clientID];
   if (player == nullptr)
   {
      addPlayer(clientID, x, y);
   }
   else
   {
      player->setPosition(sf::Vector2f(x, y));
      player->setAngle(angle);
   }
}

void ClientPlayersManager::removePlayer(const ClientID & clientID)
{
   if (this->players.size() > 0)
   {
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
}
