#include "ClientCannonballManager.h"
#include "Global.h"
#include "Utilities.h"


ClientCannonballManager::ClientCannonballManager(World &_world, sf::Mutex &_mutex)
   : world(_world), mutex(_mutex)
{
   this->Cannonballs.clear();
}


ClientCannonballManager::~ClientCannonballManager()
{
   for (auto &iter : this->Cannonballs)
   {
      DELLISNOTNULL(iter.second);
   }
   this->Cannonballs.clear();
}

void ClientCannonballManager::addCannonball(const ClientID & clientID, const float & x, const float & y)
{
   if (!this->world.getWorld()->IsLocked())
   {
      sf::Lock lock(this->mutex);
      if (this->Cannonballs.count(clientID) <= 0)
      {
         this->Cannonballs[clientID] = createCannonball(x, y, "Cannonball");
      }
   }
}

ClientCannonball * ClientCannonballManager::createCannonball(float x, float y, const std::string & textureName)
{
   this->textureManager.RequireResource(textureName);
   return new ClientCannonball(createCannonballBody(x, y), *this->textureManager.GetResource(textureName));
}

void ClientCannonballManager::addCannonball(const ClientID & clientID, ClientCannonball *player)
{
   this->Cannonballs[clientID] = player;
}

void ClientCannonballManager::moveCannonball(const CannonballState &state)
{
   if (this->Cannonballs.count(state.cannID) <= 0)
   {
      //addCannonball(state.cannID, state.x, state.y);
      //std::cout << "Test" << std::endl;
   }
   else
   {
      if (!this->world.getWorld()->IsLocked())
      {
         sf::Lock lock(this->mutex);
         b2Body *body = this->Cannonballs[state.cannID]->getBody();
         b2Vec2 pos = body->GetPosition();
         b2Vec2 vel = body->GetLinearVelocity();
         float time = state.time / 1000.0f;
         float angleOr = body->GetAngle();
         body->SetTransform({ state.x, state.y }, angleOr);
         //float angularOrVel = body->GetAngularVelocity();
         ////b2Vec2 velExtrapolation( (state.linearVelocity.x + (state.linearVelocity.x > 0 ? 1 : -1) * (state.linearVelocity.x * time) ), (state.linearVelocity.y + (state.linearVelocity.y > 0 ? 1 : -1) * state.linearVelocity.y * time) );
         ////b2Vec2 posExtrapolation( (state.x + state.x * time), (state.y + state.y * time));
         //b2Vec2 velExtrapolation((state.linearVelocity.x + (state.linearVelocity.x > 0 ? 1 : -1) * 0.5 * (state.linearVelocity.x * time * time)), (state.linearVelocity.y + (state.linearVelocity.y > 0 ? 1 : -1) * 0.5 * state.linearVelocity.y * time * time));
         ////         velExtrapolation *= state.angularVel != 0 ? state.angularVel * time * time * 0.5f : 1;
         //b2Vec2 posExtrapolation((state.x + state.x * time * time * 0.5), (state.y + state.y * time * time * 0.5));
         //bool posUpdate = false;
         ////std::cout << state.time << std::endl;
         //body->SetLinearVelocity(velExtrapolation);
         //float posDiff = b2DistanceSquared(posExtrapolation, pos);
         //if (posDiff > 0.3)// || (velExtrapolation.x <= 0.3 || velExtrapolation.y <= 0.3) && posDiff > 0.05f)
         //{
         //   bool awake = body->IsAwake();
         //   body->SetAwake(!awake);
         //   body->SetAwake(awake);
         //   body->SetTransform(posExtrapolation, angleOr);
         //   //body->SetLinearVelocity(b2Vec2(0, 0));
         //   posUpdate = true;
         //}
         //body->SetTransform(posUpdate ? posExtrapolation : pos, angleOr);
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
      this->lastUpdates[state.cannID] = 5;
   }
}

void ClientCannonballManager::removeCannonball(const CannID & cannID)
{
   if (this->Cannonballs.count(cannID) > 0)
   {
      this->world.removeBody(this->Cannonballs[cannID]->getBody());
      DELLISNOTNULL(this->Cannonballs[cannID]);
      this->Cannonballs.erase(cannID);
   }
}

ClientCannonball* ClientCannonballManager::getCannonball(const CannID & cannID)
{
   return this->Cannonballs[cannID];
}

void ClientCannonballManager::drawAllCannonbalss(Window & window)
{
   for (auto &iter : this->Cannonballs)
   {
      iter.second->draw(window);
   }
}

void ClientCannonballManager::updateAllCannonballs(const sf::Time & time)
{
   for (auto &iter : this->Cannonballs)
   {
      iter.second->update(time);
   }
   //find and erase players sprites that they not updated from snapshots
   for (auto iter = this->lastUpdates.cbegin(), next_it = iter; iter != this->lastUpdates.cend(); iter = next_it)
   {
      ++next_it;
      if (iter->second <= 0)
      {
         removeCannonball(iter->first);
         this->lastUpdates.erase(iter->first);
      }
   }

}

void ClientCannonballManager::decreaseCannonballOccurence()
{
   for (auto &iter : this->lastUpdates)
   {
      iter.second--;
   }
}


b2Body * ClientCannonballManager::createCannonballBody(float x, float y)
{
   b2Body *body = this->world.createBody(x, y);
   this->world.createCicleFixture(body, 2, 1, 0.5f);
   return body;
}