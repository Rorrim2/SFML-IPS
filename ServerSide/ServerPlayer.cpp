#include "ServerPlayer.h"
#include "Global.h"
#include <iostream>
#include <cmath>

ServerPlayer::ServerPlayer(World *world, const float& x, const float& y)
{
   this->body = world->createBody(x, y);
   world->createBoxFixture(this->body, 25, 25, 1, 0.5f);
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
   if (direction & MoveDirection::FORWARD)
   {
      if(speedVec.y < 5)
         speedVec.y = 50;
   }
   else if (direction & MoveDirection::BACKWARD)
   {
      if (speedVec.y > -5)
         speedVec.y = -50;
   }

   if (direction & MoveDirection::LEFT)
   {
      if (speedVec.x > -5)
         speedVec.x = -50;
   }
   else if (direction & MoveDirection::RIGHT)
   {
      if (speedVec.x < 5)
         speedVec.x = 50;
   }
   this->body->ApplyForceToCenter(speedVec, true);
}

b2Body * ServerPlayer::getBody()
{
   return this->body;
}
