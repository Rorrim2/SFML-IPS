#include "ClientPlayer.h"
#include "Global.h"
#include <iostream>
#include <math.h>
#include "Global.h"


ClientPlayer::ClientPlayer(b2Body *body)
{
   this->maxSpeed = 15;
   //this->body = body;
   this->sprite = new sf::RectangleShape(sf::Vector2f(154, 154));
   this->sprite->setFillColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));

   this->sprite->setOrigin(this->sprite->getSize().x / 2, this->sprite->getSize().y / 2);
 //  this->sprite->setPosition(this->body->GetPosition().x * PIXELS_PER_METER, this->body->GetPosition().y * PIXELS_PER_METER);
   this->sprite->setPosition(200, 300);

   this->lastTime = sf::Time::Zero;
}


ClientPlayer::~ClientPlayer()
{
   DELLISNOTNULL(this->sprite);
}


void ClientPlayer::update(const sf::Time& time)
{
   //b2Vec2 pos = this->body->GetPosition();
   //this->sprite->setPosition(sf::Vector2f(pos.x * PIXELS_PER_METER, pos.y * PIXELS_PER_METER));
  // this->sprite->setRotation(this->body->GetAngle() / b2_pi * 180.0f);
}

void ClientPlayer::draw(Window & window)
{
   window.draw(*this->sprite);
}

void ClientPlayer::move(MoveDirection direction)
{
  // b2Vec2 speedVec = body->GetLinearVelocity();
   float x = 0, y = 0;
   if (direction & MoveDirection::FORWARD)
   {
      y = -this->maxSpeed;
   }
   else if (direction & MoveDirection::BACKWARD)
   {
      y = this->maxSpeed;
   }

   if (direction & MoveDirection::LEFT)
   {
      x = -this->maxSpeed;
   }
   else if (direction & MoveDirection::RIGHT)
   {
      x = this->maxSpeed;
   }
   //this->body->ApplyForceToCenter(b2Vec2(x - speedVec.x, y - speedVec.y), true);
   //this->body->ApplyForceToCenter(speedVec, true);
}