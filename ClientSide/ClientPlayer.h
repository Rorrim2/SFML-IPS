#pragma once
#include <SFML/Graphics.hpp>
#include "Window.h"

class ClientPlayer
{
public:
   ClientPlayer(const float& x, const float& y);
   ~ClientPlayer();


   void setPosition(const sf::Vector2f &pos);
   void setAngle(const float& angle);

   void update(const sf::Time& time);
   void lerpPos(const sf::Time &time);
   void draw(Window &window);

private:
   sf::Time lastTime;
   float angle;
   sf::Vector2f lastPos;
   sf::RectangleShape* sprite;
};

