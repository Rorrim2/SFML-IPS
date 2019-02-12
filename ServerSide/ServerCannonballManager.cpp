#include "ServerCannonballManager.h"



ServerCannonballManager::ServerCannonballManager(World &world, sf::Mutex &_mutex)
	: world(world), mutex(_mutex)
{
   this->lastIndex = 0;
}


ServerCannonballManager::~ServerCannonballManager()
{
	for (auto &iter : this->cannBalls)
	{
		DELLISNOTNULL(iter.second);
	}
	this->cannBalls.clear();
}

ServerCannonball * ServerCannonballManager::getCannonBall(const CannID & cannID)
{
	return this->cannBalls[cannID];
}

ServerCann & ServerCannonballManager::getAllCannoballs()
{
	return this->cannBalls;
}

ServerCannonball* ServerCannonballManager::addCannonball(const float & x, const float & y)
{
	//TODO: one cannB if if is active, think about it
	//if (this->cannBalls.count(cannID) <= 0)
   {
      sf::Lock lock(this->mutex);
      this->cannBalls[this->lastIndex] = new ServerCannonball(createCannonballBody(x, y));
	}
   return this->cannBalls[this->lastIndex++];
}

void ServerCannonballManager::update(const sf::Time &time)
{
   for (auto iter = this->cannBalls.cbegin(), next_it = iter; iter != this->cannBalls.cend(); iter = next_it)
   {
      ++next_it;
      iter->second->update(time);
      if (iter->second->isDestroy())
      {
         removeCannonball(iter->first);
      }
   }
}
void ServerCannonballManager::removeCannonball(const CannID cannID)
{
	if (this->cannBalls.find(cannID) != this->cannBalls.end())
	{
		this->world.removeBody(this->cannBalls[cannID]->getBody());
		DELLISNOTNULL(this->cannBalls[cannID]);
		this->cannBalls.erase(cannID);
	}
}

b2Body* ServerCannonballManager::createCannonballBody(float x, float y)
{
	b2Body* body = this->world.createBody(x, y);
	this->world.createCicleFixture(body, 2, 1, 0.5f);
	return body;
}


