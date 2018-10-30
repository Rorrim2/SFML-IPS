#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D.h>
#include <Window.h>
#include <World.h>
#include <PacketType.h>

class ServerPlayer
{
public:
   ServerPlayer(World *world, const sf::Vector2f &p);
	ServerPlayer(World *world, const float& x, const float& y);
   ~ServerPlayer();

   sf::Vector2f getPosition();
   float getAngle();

   void move(const float &speed, MoveDirection &direction);

   b2Body* getBody();
private:
   //TODO add max speed
   b2Body* body;
};

