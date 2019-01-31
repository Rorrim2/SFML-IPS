#include "ClientPlayer.h"
#include "Global.h"
#include <iostream>
#include <math.h>
#include "Global.h"


ClientPlayer::ClientPlayer(b2Body *body, const sf::Texture & texture)
   : sprite(texture)
{
   this->maxSpeed = 2;
   this->maxAngularSpeed = 0.5;
   this->verticalSpeed = 0;
   this->angularSpeed = 0;
   this->body = body;
   this->sprite.setScale(.7, .7);
   //this->sprite = new sf::RectangleShape(sf::Vector2f(52, 52));
   //this->sprite->setFillColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));

   this->sprite.setOrigin(56, 33);
   this->sprite.setPosition(this->body->GetPosition().x * PIXELS_PER_METER, this->body->GetPosition().y * PIXELS_PER_METER);

   this->lastTime = sf::Time::Zero;
}


ClientPlayer::~ClientPlayer()
{
}


void ClientPlayer::update(const sf::Time& time)
{
   b2Vec2 pos = this->body->GetPosition();
   this->sprite.setPosition(sf::Vector2f(pos.x * PIXELS_PER_METER, pos.y * PIXELS_PER_METER));
   this->sprite.setRotation(this->body->GetAngle() / b2_pi * 180.0f);
   this->verticalSpeed *= .78;
   this->angularSpeed *= .50;
   //this->body->SetAngularVelocity(this->angularSpeed);

}

void ClientPlayer::draw(Window & window)
{
   window.draw(this->sprite);
}

void ClientPlayer::move(MoveDirection direction)
{
   //b2Vec2 speedVec = this->body->GetLinearVelocity();
   //float angular = this->body->GetAngularVelocity();
   //b2Vec2 angularVec = this->body->GetLinearVelocity();
   //float x = std::cos(this->body->GetAngle());
   //float y = std::sin(this->body->GetAngle());
   //std::cout << "ruch" << std::endl;
   //if (direction & MoveDirection::FORWARD)
   //{
   //   if (std::fabs(this->verticalSpeed) <= this->maxSpeed)
   //   {
   //      this->verticalSpeed += 0.2f;
   //      this->body->SetLinearVelocity(b2Vec2(x * verticalSpeed, this->verticalSpeed * y));
   //   }
   //}
   //else if (direction & MoveDirection::BACKWARD)
   //{
   //   if (std::fabs(this->verticalSpeed) <= this->maxSpeed)
   //   {
   //      //this->verticalSpeed -= 0.2f;
   //   }
   //}

   if (direction & MoveDirection::LEFT)
   {
      if (std::fabs(this->angularSpeed) <= this->maxAngularSpeed)
      {
         this->angularSpeed += 0.4f;
      }
   }
   else if (direction & MoveDirection::RIGHT)
   {
      if (std::fabs(this->angularSpeed) <= this->maxAngularSpeed)
      {
         this->angularSpeed -= 0.4f;
      }
   }
}

b2Body * ClientPlayer::getBody()
{
   return this->body;
}
