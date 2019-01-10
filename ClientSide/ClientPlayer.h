#pragma once
#include <SFML/Graphics.hpp>
#include "Window.h"
#include <Box2D.h>
#include "PacketType.h"
#include "TextureManager.h"


class ClientPlayer
{
public:
   ClientPlayer(b2Body *body, const sf::Texture & texture);
   ~ClientPlayer();

   void update(const sf::Time& time);
   void draw(Window &window);
   void move(MoveDirection direction);

   b2Body* getBody();
private:
   b2Body *body;
   float maxSpeed;
   float maxAngularSpeed;
   float verticalSpeed;
   float angularSpeed;

   sf::Sprite sprite;
   sf::Time lastTime;
   float angle;
   sf::Vector2f lastPos;
};

