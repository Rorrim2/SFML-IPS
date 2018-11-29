#include "ServerPlayer.h"
#include "Global.h"
#include <iostream>
#include <cmath>

ServerPlayer::ServerPlayer(World *world, const float& x, const float& y)
{
   this->body = world->createBody(x, y);
   world->createBoxFixture(this->body, 25, 25, 1, 0.5f);
   this->maxSpeed = 15;
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

void ServerPlayer::move(const float & speed, MoveDirection & direction)
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
   this->body->ApplyForceToCenter(b2Vec2(x - speedVec.x, y - speedVec.y), true);
   //this->body->ApplyForceToCenter(speedVec, true);
}

b2Body * ServerPlayer::getBody()
{
   return this->body;
}
