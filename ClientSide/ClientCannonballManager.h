#pragma once
#include <unordered_map>
#include "ClientCannonball.h"
#include "Network.h"
#include "World.h"

using CannID = int;

struct CannonballState
{
	int time;
	CannID cannID;
	float x;
	float y;
	b2Vec2 linearVelocity;
};

class ClientCannonballManager
{
public:
	ClientCannonballManager(World &world, sf::Mutex &mutex);
	~ClientCannonballManager();

	ClientCannonball* createCannonball(float x, float y, const std::string & texture = "");
	void addCannonball(const CannID & cannID, ClientCannonball* cannonball);
	void addCannonball(const CannID & cannID, const float &x, const float &y);
	void moveCannonball(const CannonballState &state);
	void removeCannonball(const CannID& cannID);

	ClientCannonball* getCannonball(const CannID& cannID);

	void drawAllCannonbalss(Window & window);
	void updateAllCannonballs(const sf::Time & time);
	void decreaseCannonballOccurence();

	b2Body* createCannonballBody(float x, float y);

private:
	TextureManager textureManager;
	sf::Mutex & mutex;
	World & world;
	std::unordered_map<CannID, ClientCannonball*> Cannonballs;
	std::unordered_map<CannID, uint8_t> lastUpdates;
};

