#pragma once
#include <SFML/System/Time.hpp>
#include "ServerPlayer.h"
#include "Window.h"
#include <unordered_map>
#include "Network.h"

using ServerPlayers = std::unordered_map<ClientID, ServerPlayer*>;

class ServerPlayersManager
{
public:
   ServerPlayersManager(World &world, sf::Mutex &mutex);
   ~ServerPlayersManager();

   ServerPlayer* getPlayer(const ClientID& clientID);
   ServerPlayers& getAllPlayers();

   void addPlayer(const ClientID& clientID, const float &x, const float &y, ShipType shipType);
   void update(const sf::Time &time);
   void removePlayer(const ClientID& clientID);
   void movePlayer(const ClientID& id, MoveDirection dir, const sf::Int32 & time);
   b2Body *createShipBody(float x, float y);

   void removeAllPlayers();

private: 
   sf::Mutex &mutex;
   World &world;
   ServerPlayers players;
};

