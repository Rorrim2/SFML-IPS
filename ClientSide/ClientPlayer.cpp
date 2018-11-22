#include "ClientPlayer.h"
#include "Global.h"
#include <iostream>
#include <math.h>

ClientPlayer::ClientPlayer(const float& x, const float& y)
{
   this->sprite = new sf::RectangleShape(sf::Vector2f(54, 54));
   this->sprite->setFillColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));

   this->sprite->setOrigin(this->sprite->getSize().x / 2, this->sprite->getSize().y / 2);
   this->sprite->setPosition(x, y);

   this->lastTime = sf::Time::Zero;
}


ClientPlayer::~ClientPlayer()
{
   DELLISNOTNULL(this->sprite);
}

void ClientPlayer::setPosition(const sf::Vector2f & pos)
{
   this->lastPos = pos;
   this->sprite->setPosition(pos);
}
void ClientPlayer::setAngle(const float& angle)
{
   this->angle = angle;
   this->sprite->setRotation(angle);
}

void ClientPlayer::update(const sf::Time& time)
{  
}
static void lerp(sf::Vector2f& res, const sf::Vector2f& a, const sf::Vector2f& b, float t)
{
   res.x = (1 - t)*a.x + t * b.x;
   res.y = (1 - t)*a.y + t * b.y;
};
void ClientPlayer::lerpPos(const sf::Time &time)
{
   sf::Vector2f res;
   lerp(res, this->sprite->getPosition(), this->lastPos, (time.asSeconds() - this->lastTime.asSeconds()));
//   std::cout << position.x << " " << position.y << std::endl;
   this->lastTime = time;
   this->sprite->setPosition(res);
   
}

void ClientPlayer::draw(Window & window)
{
   window.draw(*this->sprite);
}
