#include "ServerCannonball.h"
#define CANNONBALL_LIFE_TIME 5


ServerCannonball::ServerCannonball(b2Body* body)
{
	this->body = body;
   this->toRemove = false;
   this->lifeTime = 0;
}


ServerCannonball::~ServerCannonball()
{
}

sf::Vector2f ServerCannonball::getPosition()
{
	b2Vec2 pos = this->body->GetPosition();
	return sf::Vector2f(pos.x, pos.y);
}

short ServerCannonball::getDamage()
{	
	return this->damage;
}

void ServerCannonball::update(const sf::Time & time)
{
   this->lifeTime += time.asSeconds();

   if (this->lifeTime >= CANNONBALL_LIFE_TIME)
   {
      this->toRemove = true;
   }
}

void ServerCannonball::setDamage(short new_damage)
{
	this->damage = new_damage;
}

CannonballState ServerCannonball::getCannState()
{
	return CannonballState(getPosition(), this->damage);
}


b2Body* ServerCannonball::getBody()
{
	return this->body;
}

bool ServerCannonball::isDestroy()
{
   return this->toRemove;
}
