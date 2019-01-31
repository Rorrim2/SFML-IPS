#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "Window.h"
#include "World.h"
#include "PacketType.h"

struct PlayerState
{
   PlayerState(const sf::Vector2f &playerCoords, float playerAngle, short playerHealth)
      : coords(playerCoords), angle(playerAngle), health(playerHealth) {}
   sf::Vector2f coords;
   float angle;
   short health;
};

class ServerPlayer
{
public:
	ServerPlayer(b2Body *body);
   ~ServerPlayer();

   sf::Vector2f getPosition();
   float getAngle();
   short getHealt();
   void setHealth(short new_heath);

   bool isDead();

   PlayerState getPlayerState();
   void update(const sf::Time &time);
   void move(MoveDirection &direction, const sf::Int32 & time);

   void shoot();
   bool canShoot();

   b2Body* getBody();
private:
   float shootTimeout;
	short health;
   float maxSpeed;
   float maxAngularSpeed;
   float verticalSpeed;
   float angularSpeed;

   b2Body* body;
};

