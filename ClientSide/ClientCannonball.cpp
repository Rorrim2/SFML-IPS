#include "ClientCannonball.h"


ClientCannonball::ClientCannonball(b2Body* body, const sf::Texture & texture)
	: sprite(texture)
{
	this->body = body;
	this->sprite.setPosition(this->body->GetPosition().x * PIXELS_PER_METER, this->body->GetPosition().y * PIXELS_PER_METER);

	this->lastTime = sf::Time::Zero;
}


ClientCannonball::~ClientCannonball()
{
}

void ClientCannonball::update(const sf::Time& time) 
{
	b2Vec2 pos = this->body->GetPosition();
	this->sprite.setPosition(sf::Vector2f(pos.x * PIXELS_PER_METER, pos.y * PIXELS_PER_METER));
}

void ClientCannonball::draw(Window & window)
{
	window.draw(this->sprite);
}

void move(MoveDirection direction) {
	
}

b2Body* ClientCannonball::getBody()
{
	return this->body;
}