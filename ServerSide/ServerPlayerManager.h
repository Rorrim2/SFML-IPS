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
   ServerPlayersManager(World &world);
   ~ServerPlayersManager();

   ServerPlayer* getPlayer(const ClientID& clientID);
   ServerPlayers& getAllPlayers();

   void addPlayer(const ClientID& clientID, const float &x, const float &y);

   void removePlayer(const ClientID& clientID);
   void movePlayer(const ClientID& id, MoveDirection dir);


private: 
   World &box2DWorld;
   ServerPlayers players;
};

