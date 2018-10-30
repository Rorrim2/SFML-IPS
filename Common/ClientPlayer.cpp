#include "ClientPlayer.h"
#include <Global.h>
#include <iostream>

int r = 0;
ClientPlayer::ClientPlayer(const float& x, const float& y)
{
   this->sprite = new sf::RectangleShape(sf::Vector2f(54, 54));
   if(r++ % 2)
      this->sprite->setFillColor(sf::Color::Yellow);
   else 
      this->sprite->setFillColor(sf::Color::Red);
   this->sprite->setOrigin(this->sprite->getSize().x / 2, this->sprite->getSize().y / 2);
   this->sprite->setPosition(x, y);
}


ClientPlayer::~ClientPlayer()
{
   DELLISNOTNULL(this->sprite);
}

void ClientPlayer::setPosition(const sf::Vector2f & pos)
{
   this->sprite->setPosition(pos);
}
void ClientPlayer::setAngle(const float& angle)
{
   this->sprite->setRotation(angle);
}

void ClientPlayer::update(const sf::Time& time)
{
   
}

void ClientPlayer::draw(Window & window)
{
   window.draw(*this->sprite);
}
