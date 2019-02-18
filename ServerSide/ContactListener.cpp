#include "ContactListener.h"
#include <ServerCannonball.h>
#include <ServerPlayer.h>


ContactListener::ContactListener()
{
}

void ContactListener::BeginContact(b2Contact * contact)
{
   ServerCannonball* ball = nullptr;
   ServerPlayer* player = nullptr;
   void* bodyUserDataA = contact->GetFixtureA()->GetBody()->GetUserData();
   void* bodyUserDataB = contact->GetFixtureB()->GetBody()->GetUserData();

   if (bodyUserDataA && static_cast<ServerCannonball*>(bodyUserDataA)->type == BodyType::CannonBall &&
      bodyUserDataB && static_cast<ServerPlayer*>(bodyUserDataB)->type == BodyType::Ship)
   {
      ball = static_cast<ServerCannonball*>(bodyUserDataA);
      player = static_cast<ServerPlayer*>(bodyUserDataB);
   }

   if (bodyUserDataB && static_cast<ServerCannonball*>(bodyUserDataB)->type == BodyType::CannonBall &&
      bodyUserDataA && static_cast<ServerPlayer*>(bodyUserDataA)->type == BodyType::Ship)
   {
      ball = static_cast<ServerCannonball*>(bodyUserDataB);
      player = static_cast<ServerPlayer*>(bodyUserDataA);
   }

   if (ball && player)
   {
      if (ball->owner != player)
      {
         ball->setToRemove();
         player->setHealth(-1);
      }
   }
}


ContactListener::~ContactListener()
{
}
