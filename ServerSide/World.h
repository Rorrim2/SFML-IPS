#pragma once
#include <Box2D/Box2D.h>
#include <SFML/System/Vector2.hpp>
#include "DebugDraw.h"
#include "Window.h"
#include "Global.h"
#include "MapParser.h"
#include <stdexcept>


//TODO add more types of bodies, add destroy body, add destroy world or stop; add possibility to change gravity


class World
{
public:
   World();
   ~World();
   
   void initWorld();
   void loadMap(const std::string &path);
   void addRectangles();
   void addPolygons();
   b2World* getWorld();

   void updateWorld();

   b2Body* createBody(const float &x, const float &y, bool isDynamic = true);
   void createBoxFixture(b2Body *body, const float &width, const float &height, const float &friction = 1, const float &density = 1);


   void initDebugDrawing(Window &window);
   void drawDebugData();

   void removeBody(b2Body *body);
   void eraseDeathBodies();

private:
	MapServerSide map;
	float x_ratio = 900.f / 1120.f;
	float y_ratio = 800.f / 1120.f;
	std::vector<b2Body*> polyBodies;
	std::vector<b2Body*> rectBodies;
	std::vector<b2Body*> bodiesToDelete;
	DebugDraw *debugDraw;
	b2World *world;
	b2Body *worldBox;
};

