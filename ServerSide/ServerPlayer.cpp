#include "ServerPlayer.h"
#include "Global.h"
#include <iostream>
#include <cmath>

ServerPlayer::ServerPlayer(b2Body *body)
{
   this->body = body;
   this->maxSpeed = 2;
   this->maxAngularSpeed = 0.5;
   this->verticalSpeed = 0;
   this->angularSpeed = 0;
   this->shootTimeout = 0;
}


ServerPlayer::~ServerPlayer()
{
}

sf::Vector2f ServerPlayer::getPosition()
{
   b2Vec2 pos = this->body->GetPosition();
   return sf::Vector2f(pos.x, pos.y);
}

float ServerPlayer::getAngle()
{
   return this->body->GetAngle();
}

short ServerPlayer::getHealt()
{
	return this->health;
}

void ServerPlayer::setHealth(short new_heath)
{
	this->health = new_heath;
}

bool ServerPlayer::isDead() 
{
	if (!this->health)
	{
		return 1;
	}
	return 0;
}

PlayerState ServerPlayer::getPlayerState()
{
   return PlayerState(getPosition(), getAngle(), 0);
}

void ServerPlayer::update(const sf::Time &time)
{
   this->verticalSpeed *= .78;
   this->angularSpeed *= .50;
   this->body->SetAngularVelocity(this->angularSpeed);
   this->shootTimeout += time.asSeconds();
}

void ServerPlayer::move(MoveDirection & direction, const sf::Int32 & _time)
{
   b2Vec2 speedVec = this->body->GetLinearVelocity();
   float angular = this->body->GetAngularVelocity();
   b2Vec2 angularVec = this->body->GetLinearVelocity();
   float x = std::cos(this->body->GetAngle());
   float y = std::sin(this->body->GetAngle());
   float time = _time / 1000.f;
   if (direction & MoveDirection::FORWARD)
   {
      if (std::fabs(this->verticalSpeed) <= this->maxSpeed)
      {
         this->verticalSpeed += 0.2f;
         this->body->SetLinearVelocity(b2Vec2(x * verticalSpeed, this->verticalSpeed * y));
      }
   }
   else if (direction & MoveDirection::BACKWARD)
   {
      if (std::fabs(this->verticalSpeed) <= this->maxSpeed)
      {
         //this->verticalSpeed -= 0.2f;
      }
   }

   if (direction & MoveDirection::LEFT)
   {
      if (std::fabs(this->angularSpeed) <= this->maxAngularSpeed)
      {
         this->angularSpeed += 0.4f + 0.4f * (time * time * 0.5f);
      }
   }
   else if (direction & MoveDirection::RIGHT)
   {
      if (std::fabs(this->angularSpeed) <= this->maxAngularSpeed)
      {
         this->angularSpeed -= 0.4f + 0.4f * (time * time * 0.5f);
      }
   }

   std:cout << "add " << 0.4f * _time * _time * 0.5f << " time :" << _time << std::endl;

}

bool ServerPlayer::canShoot()
{
   return this->shootTimeout > 2.f;
}
void ServerPlayer::shoot()
{
   this->shootTimeout = 0;
}

b2Body * ServerPlayer::getBody()
{
   return this->body;
}
