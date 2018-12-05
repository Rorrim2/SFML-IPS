#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "Window.h"
#include "World.h"
#include "PacketType.h"

struct PlayerState
{
   PlayerState(const sf::Vector2f &playerCoords, float playerAngle, int playerHealth)
      : coords(playerCoords), angle(playerAngle), health(playerHealth) {}
   sf::Vector2f coords;
   float angle;
   int health;
};

class ServerPlayer
{
public:
   ServerPlayer(World *world, const sf::Vector2f &p);
	ServerPlayer(World *world, const float& x, const float& y);
   ~ServerPlayer();

   sf::Vector2f getPosition();
   float getAngle();

   PlayerState getPlayerState();

   void move(MoveDirection &direction);

   b2Body* getBody();
private:
   //TODO add max speed
   float maxSpeed;
   b2Body* body;
};

