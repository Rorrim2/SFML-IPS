#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "Window.h"
#include "World.h"
#include "ServerPlayer.h"
#include "PacketType.h"

struct CannonballState
{
	CannonballState(const sf::Vector2f &cannCoords, short cannDamage)
		: coords(cannCoords), damage(cannDamage) {}
	sf::Vector2f coords;
	short damage;
};

class ServerCannonball
{
public:
	ServerCannonball(b2Body* body, const ServerPlayer * owner);
	~ServerCannonball();

	sf::Vector2f getPosition();
	short getDamage();
   void update(const sf::Time &time);
	void setDamage(short new_damage);
   void setToRemove();
	CannonballState getCannState();

	b2Body* getBody();

   bool isDestroy();
   BodyType type = CannonBall;
   const ServerPlayer * owner;
private:
   float lifeTime;
   bool toRemove;
	short damage;
	b2Body* body;
};

