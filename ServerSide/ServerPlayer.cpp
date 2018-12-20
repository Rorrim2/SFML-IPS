#include "ServerPlayer.h"
#include "Global.h"
#include <iostream>
#include <cmath>

ServerPlayer::ServerPlayer(b2Body *body)
{
   this->body = body;
   this->maxSpeed = 5;
}


ServerPlayer::~ServerPlayer()
{
}

sf::Vector2f ServerPlayer::getPosition()
{
   b2Vec2 pos = this->body->GetPosition();
   return sf::Vector2f(pos.x * PIXELS_PER_METER, pos.y * PIXELS_PER_METER);
}

float ServerPlayer::getAngle()
{
   return this->body->GetAngle() / b2_pi * 180.0f;
}

PlayerState ServerPlayer::getPlayerState()
{
   return PlayerState(getPosition(), getAngle(), 0);
}

void ServerPlayer::move(MoveDirection & direction)
{
   b2Vec2 speedVec = body->GetLinearVelocity();
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
   b2Vec2 impulse = b2Vec2(x, y) - speedVec;
   impulse *= this->body->GetMass();

   this->body->ApplyLinearImpulseToCenter(impulse, true);
   //this->body->ApplyForceToCenter(speedVec, true);
}

b2Body * ServerPlayer::getBody()
{
   return this->body;
}
