#pragma once
#include <SFML/Graphics.hpp>
#include "Window.h"
#include <Box2D.h>
#include "PacketType.h"

class ClientPlayer
{
public:
   ClientPlayer(b2Body *body);
   ~ClientPlayer();

   void update(const sf::Time& time);
   void draw(Window &window);
   void move(MoveDirection direction);

private:
   b2Body *body;
   float maxSpeed;

   sf::Time lastTime;
   float angle;
   sf::Vector2f lastPos;
   sf::RectangleShape* sprite;
};

