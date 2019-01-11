#include "World.h"
#include "Global.h"
#include <SFML/Window/Event.hpp>
#include "MapParser.h"

World::World()
{
}

World::~World()
{
}

void World::initWorld()
{
   this->world = new b2World(b2Vec2(0, 0));
   //800x900
   //this is temporary world egdes
   //this->worldBox = createBody(450, 790, false);
   //createBoxFixture(this->worldBox, 450, 10);

   //this->worldBox = createBody(450, 10, false);
   //createBoxFixture(this->worldBox, 450, 10);

   //this->worldBox = createBody(10, 400, false);
   //createBoxFixture(this->worldBox, 10, 400);

   //this->worldBox = createBody(890, 400, false);
   //createBoxFixture(this->worldBox, 10, 400);
}

void World::loadMap(const std::string &path)
{
   bool didItWorked = map.loadFromFile(path);
   if (didItWorked == false) {
      cout << "Failed to load map" << endl;
   }
}

void World::addRectangles()
{
   std::vector<Rectangle> Rects = map.getRectVector();
   //cout << "Rects size: " << Rects.size() << endl;
   for (int i = 0; i < Rects.size(); ++i)
   {
      if (Rects[i].type == DYNAMIC) {
         b2Body* temp = createBody(Rects[i].x * x_ratio + Rects[i].width / 2 * x_ratio, Rects[i].y * y_ratio + Rects[i].height / 2 * y_ratio, true);
         createBoxFixture(temp, Rects[i].width / 2 * x_ratio, Rects[i].height / 2 * y_ratio);
         rectBodies.push_back(temp);
      }
      else if (Rects[i].type == STATIC) {
         b2Body* temp = createBody(Rects[i].x * x_ratio + Rects[i].width / 2 * x_ratio, Rects[i].y * y_ratio + Rects[i].height / 2 * y_ratio, false);
         createBoxFixture(temp, Rects[i].width / 2 * x_ratio, Rects[i].height / 2 * y_ratio);
         rectBodies.push_back(temp);
      }
      else {
         cout << "Something went wrong in addRectangles" << endl;
         return;
      }
   }

}

void World::addPolygons()
{
   std::vector<Polygon> Poly = map.getPolyVector();
   b2BodyDef bodyDef;
   //cout << "Poly size: " << Poly.size() << endl;
   for (int i = 0; i < Poly.size(); ++i)
   {
      if (Poly[i].type == DYNAMIC)
      {
         bodyDef.type = b2_dynamicBody;
      }
      else if (Poly[i].type == STATIC)
      {
         bodyDef.type = b2_staticBody;
      }
      else
      {
         cout << "Something went wrong in addPolygons" << endl;
         return;
      }
      size_t num_of_points = Poly[i].points.size();
      b2Body* body = this->world->CreateBody(&bodyDef);
      b2Vec2* vertices = new b2Vec2[num_of_points];
      b2Vec2 temp_point0;
      temp_point0.x = Poly[i].x;
      temp_point0.y = Poly[i].y;
      b2Vec2 temp_point;
      for (int j = 0; j < num_of_points; ++j)
      {
         temp_point.x = (Poly[i].points[j].y + temp_point0.x) * METERS_PER_PIXEL * x_ratio;
         temp_point.y = (Poly[i].points[j].x + temp_point0.y) * METERS_PER_PIXEL * y_ratio;
         vertices[j] = temp_point;
      }
      b2ChainShape chain;
      chain.CreateLoop(vertices, num_of_points);
      b2FixtureDef fixtureDef;
      fixtureDef.shape = &chain;
      fixtureDef.density = 1;
      fixtureDef.friction = 1;
      fixtureDef.restitution = 0.5f;
      body->CreateFixture(&fixtureDef);
      polyBodies.push_back(body);
      //delete[] vertices;
   }
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

