#pragma once
#include <Box2D/Box2D.h>
#include <SFML/System/Vector2.hpp>
#include "DebugDraw.h"
#include "Window.h"
#include "Global.h"
#include <stdexcept>


//TODO add more types of bodies, add destroy body, add destroy world or stop; add possibility to change gravity


class World
{
public:
   World();
   ~World();
   
   
   void initWorld();
   b2World* getWorld();

   void updateWorld();

   b2Body* createBody(const float &x, const float &y, bool isDynamic = true);
   void createBoxFixture(b2Body *body, const float &width, const float &height, const float &friction = 1, const float &density = 1);


   void initDebugDrawing(Window &window);
   void drawDebugData();

   void removeBody(b2Body *body);
   void eraseDeathBodies();

private:
   std::vector<b2Body*> bodiesToDelete;
   DebugDraw *debugDraw;
   b2World *world;
   b2Body *worldBox;
};

