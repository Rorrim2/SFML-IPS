#include "World.h"
#include "Global.h"
#include <SFML/Window/Event.hpp>

World::World()
{
}

World::~World()
{
}

void World::initWorld()
{
   this->world = new b2World(b2Vec2(0, 0));

   //this is temporary world egdes
   this->worldBox = createBody(400, 800, false);
   createBoxFixture(this->worldBox, 400, 50);
   this->worldBox = createBody(400, 0, false);
   createBoxFixture(this->worldBox, 400, 50);
   this->worldBox = createBody(0, 400, false);
   createBoxFixture(this->worldBox, 50, 400);
   this->worldBox = createBody(800, 400, false);
   createBoxFixture(this->worldBox, 50, 400);
}

b2World * World::getWorld()
{
   return this->world;
}

void World::updateWorld()
{
   if (this->world == nullptr) throw std::runtime_error("Box2D world is uninitialize!");
   this->world->Step(1 / 30.f, 8, 3);
}

b2Body* World::createBody(const float &x, const float &y, bool isDynamic)
{
   b2BodyDef bodyDef;
   if (isDynamic)
   {
      bodyDef.type = b2_dynamicBody;
   }
   else
   {
      bodyDef.type = b2_staticBody;
   }
   bodyDef.position.Set(x * METERS_PER_PIXEL, y * METERS_PER_PIXEL);

   return this->world->CreateBody(&bodyDef);
}

void World::createBoxFixture(b2Body *body, const float & width, const float & height, const float & friction, const float & density)
{
   b2PolygonShape shape;
   shape.SetAsBox(width * METERS_PER_PIXEL, height * METERS_PER_PIXEL);
   b2FixtureDef fixtureDef;
   fixtureDef.shape = &shape;
   fixtureDef.density = density;
   fixtureDef.friction = friction;
   fixtureDef.restitution = 0.5f;
   body->CreateFixture(&fixtureDef);
}

void World::initDebugDrawing(Window &window)
{
   this->debugDraw = new DebugDraw(window);
   this->debugDraw->SetFlags(b2Draw::e_shapeBit);
   this->world->SetDebugDraw(this->debugDraw);
}
void World::drawDebugData()
{
   this->world->DrawDebugData();
}

void World::removeBody(b2Body * body)
{
   this->bodiesToDelete.push_back(body);
}

void World::eraseDeathBodies()
{
   for (b2Body *b : this->bodiesToDelete)
   {
      this->world->DestroyBody(b);
   }
   this->bodiesToDelete.clear();
}

