#include "ClientPlayersManager.h"
#include "Global.h"


ClientPlayersManager::ClientPlayersManager(World &_world, sf::Mutex &_mutex)
   : world(_world), mutex(_mutex)
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
   if (!this->world.getWorld()->IsLocked())
   {
      sf::Lock lock(this->mutex);
      if (this->players.count(clientID) <= 0)
      {
         this->players[clientID] = new ClientPlayer(createShipBody(x, y));
      }
   }
}
void ClientPlayersManager::addPlayer(const ClientID & clientID, ClientPlayer *player)
{
   this->players[clientID] = player;
}
void ClientPlayersManager::movePlayer(const ClientID & clientID, const float & x, const float & y, const float & angle, b2Vec2 linearVelocity)
{
   if (this->players.count(clientID) <= 0)
   {
      addPlayer(clientID, x, y);
   }
   else
   {
      if (!this->world.getWorld()->IsLocked())
      {
         sf::Lock lock(this->mutex);
         b2Vec2 pos = this->players[clientID]->getBody()->GetPosition();
         b2Vec2 vel = this->players[clientID]->getBody()->GetLinearVelocity();
         float angleOr = this->players[clientID]->getBody()->GetAngle();
         if (std::fabs(x * METERS_PER_PIXEL - pos.x) >= 1 || std::fabs(y * METERS_PER_PIXEL - pos.y) >= 1)
         {
            this->players[clientID]->getBody()->SetTransform(b2Vec2(x * METERS_PER_PIXEL, y * METERS_PER_PIXEL), this->players[clientID]->getBody()->GetTransform().q.GetAngle());
            std::cout << "update" << std::endl;
         }
         if (std::fabs(angle * b2_pi / 180.0f - angleOr) >= .2)
         {
            this->players[clientID]->getBody()->SetTransform(this->players[clientID]->getBody()->GetTransform().p, angle * b2_pi / 180.0f);
            std::cout << "angle" << std::endl;
         }
         if (std::fabs(vel.x - linearVelocity.y) >= 1 || std::fabs(vel.y - linearVelocity.y) >= 4)
         {
            this->players[clientID]->getBody()->SetLinearVelocity(linearVelocity);
            std::cout << "update speed" << std::endl;
         }

      }
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
   for (auto iter = this->lastUpdates.cbegin(), next_it = iter; iter != this->lastUpdates.cend(); iter = next_it)
   {
      ++next_it;
      if(iter->second <= 0)
      {
         removePlayer(iter->first);
         this->lastUpdates.erase(iter->first);
      }
   }

}

void ClientPlayersManager::decreasePlayerOccurence()
{
   for (auto &iter : this->lastUpdates)
   {
      iter.second--;
   }
}


b2Body * ClientPlayersManager::createShipBody(float x, float y)
{
   b2Body *body = this->world.createBody(x, y);
   this->world.createBoxFixture(body, 25, 25, 1, 0.5f);
   return body;
}
