#pragma once
#include <SFML/Graphics.hpp>
#include "Window.h"
#include <Box2D.h>
#include "PacketType.h"
#include "TextureManager.h"


class ClientPlayer
{
public:
   ClientPlayer(b2Body *body, const sf::Texture & texture, ShipType type);
   ~ClientPlayer();

   void update(const sf::Time& time);
   void draw(Window &window);
   void move(MoveDirection direction);

   void setHealth(short health);
   short getHealth();

   bool canShoot();
   b2Body* getBody();

   const ShipType shipType;
private:
   b2Body *body;
   float maxSpeed;
   float maxAngularSpeed;
   float verticalSpeed;
   float angularSpeed;

   short health = 3;
   sf::Sprite sprite;
   sf::Time lastTime;
   float angle;
   sf::Vector2f lastPos;
   float shootTimeout;

};

