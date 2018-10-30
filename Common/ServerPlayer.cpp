#include "ServerPlayer.h"
#include <Global.h>
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

void ServerPlayer::move(const float & speed, MoveDirection & direction)
{
   b2Vec2 speedVec;
   if (direction & MoveDirection::FORWARD)
   {
      speedVec.y = speed;
   }
   else if (direction & MoveDirection::BACKWARD)
   {
      speedVec.y = -speed;
   }

   if (direction & MoveDirection::LEFT)
   {
      speedVec.x = -speed;
   }
   else if (direction & MoveDirection::RIGHT)
   {
      speedVec.x = speed;
   }
   this->body->SetLinearVelocity(speedVec);
}

b2Body * ServerPlayer::getBody()
{
   return this->body;
}
