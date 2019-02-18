#pragma once
#include <unordered_map>
#include "ClientPlayer.h"
#include "Network.h"
#include "World.h"
struct PlayerState
{
   int time;
   ClientID clientID;
   float x;
   float y;
   float angle;
   float angularVel;
   b2Vec2 linearVelocity;
   ShipType type;
};

class ClientPlayersManager
{
public:
   ClientPlayersManager(World &world, sf::Mutex &mutex);
   ~ClientPlayersManager();

   ClientPlayer* createPlayer(float x, float y, ShipType type);
   void addPlayer(const ClientID & clientID, ClientPlayer *player);
   void addPlayer(const ClientID& clientID, const float &x, const float &y, ShipType type);
   void movePlayer(const PlayerState &state);
   void removePlayer(const ClientID& clientID);
   void updateHealth(const ClientID& clientID, short health);

   ClientPlayer* getPlayer(const ClientID& clientID);

   void drawAllPlayers(Window &window);
   void updateAllPlayers(const sf::Time& time);
   void decreasePlayerOccurence();

   b2Body* createShipBody(float x, float y);

private:
   TextureManager textureManager;
   sf::Mutex &mutex;
   World &world;
   std::unordered_map<ClientID, ClientPlayer*> players;
   std::unordered_map<ClientID, uint8_t> lastUpdates; // starting from 5 - if reach 0 it won't be drawn on screen and body will be erase from client world
};

