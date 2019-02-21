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

void ClientPlayersManager::addPlayer(const ClientID & clientID, const float & x, const float & y, ShipType type)
{
   if (!this->world.getWorld()->IsLocked())
   {
      sf::Lock lock(this->mutex);
      if (this->players.count(clientID) <= 0)
      {
         this->players[clientID] = createPlayer(x, y, type);
      }
   }
}
ClientPlayer * ClientPlayersManager::createPlayer(float x, float y, ShipType type)
{
   std::string textureName;
   switch (type)
   {
   case ShipType::RED:
      textureName = "Ship_red";
      break;
   case ShipType::BLUE:
      textureName = "Ship_blue";
      break;
   case ShipType::YELLOW:
      textureName = "Ship_yellow";
      break;
   case ShipType::GREEN:
      textureName = "Ship_green";
      break;
   default:
      break;
   }
   this->textureManager.RequireResource(textureName);
   return new ClientPlayer(createShipBody(x, y), *this->textureManager.GetResource(textureName), type);
}
void ClientPlayersManager::addPlayer(const ClientID & clientID, ClientPlayer *player)
{
   this->players[clientID] = player;
}
void ClientPlayersManager::movePlayer(const PlayerState &state)
{
   if (this->players.count(state.clientID) <= 0)
   {
      addPlayer(state.clientID, state.x, state.y, state.type);
   }
   else
   {
      if (!this->world.getWorld()->IsLocked())
      {
         sf::Lock lock(this->mutex);
         b2Body *body = this->players[state.clientID]->getBody();
         b2Vec2 pos = body->GetPosition();
         b2Vec2 vel = body->GetLinearVelocity();
         float time = state.time / 1000.0f;
         float angleOr = body->GetAngle();
         float angularOrVel = body->GetAngularVelocity();
         float angleExtrapolation = state.angle + 0.5f * state.angle * time * time;
         float angularVelExtrapolation = state.angularVel + 0.5f * state.angularVel * time * time;
         //b2Vec2 velExtrapolation( (state.linearVelocity.x + (state.linearVelocity.x > 0 ? 1 : -1) * (state.linearVelocity.x * time) ), (state.linearVelocity.y + (state.linearVelocity.y > 0 ? 1 : -1) * state.linearVelocity.y * time) );
         //b2Vec2 posExtrapolation( (state.x + state.x * time), (state.y + state.y * time));
         b2Vec2 velExtrapolation((state.linearVelocity.x + (state.linearVelocity.x > 0 ? 1 : -1) * 0.5 * (state.linearVelocity.x * time * time)), (state.linearVelocity.y + (state.linearVelocity.y > 0 ? 1 : -1) * 0.5 * state.linearVelocity.y * time * time));
         //         velExtrapolation *= state.angularVel != 0 ? state.angularVel * time * time * 0.5f : 1;
         b2Vec2 posExtrapolation((state.x + state.x * time * time * 0.5), (state.y + state.y * time * time * 0.5));
         bool posUpdate = false;
         //std::cout << state.time << std::endl;
         body->SetAngularVelocity(angularVelExtrapolation);
         body->SetLinearVelocity(velExtrapolation);
         float posDiff = b2DistanceSquared(posExtrapolation, pos);
         if (posDiff > 0.2)// || (velExtrapolation.x <= 0.3 || velExtrapolation.y <= 0.3) && posDiff > 0.05f)
         {
            posUpdate = true;
            //std::cout << "x: " << posExtrapolation.x << "  | " << pos.x << "  | " << state.x << std::endl;
         }
         if (std::fabs(angleExtrapolation - angleOr) >= .005)
         {
            body->SetTransform(posUpdate ? posExtrapolation : pos, angleExtrapolation);
            //std::cout << "or: " << angleOr << " ex: " << angleExtrapolation <<  std::endl;
         }
         else
         {
            body->SetTransform(posUpdate ? posExtrapolation : pos, angleOr);
         }
         //if (std::fabs(angularVelExtrapolation - angularOrVel) >= .01)
         //{
         //   body->SetAngularVelocity(angularVelExtrapolation * (state.angularVel > 0 ? 1 : -1));
         //   std::cout << "angular" << std::endl;
         //}
         //if (b2DistanceSquared(velExtrapolation, vel) > 0.2)
         //{
         //   body->SetLinearVelocity(velExtrapolation);
         //   std::cout << "update speed" << std::endl;
         //}

      }
      this->lastUpdates[state.clientID] = 5;
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

void ClientPlayersManager::updateHealth(const ClientID & clientID, short health)
{
   if (this->players.count(clientID) > 0)
   {
      this->players[clientID]->setHealth(health);
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
      if (iter->second <= 0)
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
   this->world.createBoxFixture(body, 33, 17, 1, 0.5f);
   return body;
}
