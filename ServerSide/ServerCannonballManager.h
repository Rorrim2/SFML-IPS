#pragma once
#include <SFML/System/Time.hpp>
#include <ServerCannonball.h>
#include "Window.h"
#include <unordered_map>
#include "Network.h"

using CannID = int;

using ServerCann = std::unordered_map<CannID, ServerCannonball*>;

class ServerCannonballManager
{
public:
	ServerCannonballManager(World &world, sf::Mutex &_mutex);
	~ServerCannonballManager();

	ServerCannonball * getCannonBall(const CannID& cannID);
	ServerCann& getAllCannoballs();

   ServerCannonball* addCannonball(const float &x, const float &y);
	void removeCannonball(const CannID cannID);
	b2Body* createCannonballBody(float x, float y);

	void removeAllCannonballs();
   void update(const sf::Time &time);

private:
   int lastIndex;
	sf::Mutex &mutex;
	World &world;
	ServerCann cannBalls;
};

//TODO there are no damage changes