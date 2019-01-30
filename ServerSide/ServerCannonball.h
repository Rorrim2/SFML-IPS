#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "Window.h"
#include "World.h"
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
	ServerCannonball(b2Body* body);
	~ServerCannonball();

	sf::Vector2f getPosition();
	short getDamage();

	void setDamage(short new_damage);

	CannonballState getCannState();

	b2Body* getBody();

private:
	short damage;
	b2Body* body;
};

