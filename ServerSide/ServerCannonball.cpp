#include "ServerCannonball.h"



ServerCannonball::ServerCannonball(b2Body* body)
{
	this->body = body;
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