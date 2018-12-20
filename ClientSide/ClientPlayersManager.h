#pragma once
#include <unordered_map>
#include "ClientPlayer.h"
#include "Network.h"
#include "World.h"

class ClientPlayersManager
{
public:
   ClientPlayersManager(World &world, sf::Mutex &mutex);
   ~ClientPlayersManager();

   void addPlayer(const ClientID & clientID, ClientPlayer *player);
   void addPlayer(const ClientID& clientID, const float &x, const float &y);
   void movePlayer(const ClientID& clientID, const float &x, const float &y, const float & angle, b2Vec2);
   void removePlayer(const ClientID& clientID);

   ClientPlayer* getPlayer(const ClientID& clientID);

   void drawAllPlayers(Window &window);
   void updateAllPlayers(const sf::Time& time);
   void decreasePlayerOccurence();

   b2Body* createShipBody(float x, float y);
private:
   sf::Mutex &mutex;
   World &world;
   std::unordered_map<ClientID, ClientPlayer*> players;
   std::unordered_map<ClientID, uint8_t> lastUpdates; // starting from 5 - if reach 0 it won't be drawn on screen
};

