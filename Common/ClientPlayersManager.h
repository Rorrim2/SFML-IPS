#pragma once
#include <unordered_map>
#include "ClientPlayer.h"
#include "Network.h"

class ClientPlayersManager
{
public:
   ClientPlayersManager();
   ~ClientPlayersManager();

   void addPlayer(const ClientID & clientID, ClientPlayer *player);
   void addPlayer(const ClientID& clientID, const float &x, const float &y);
   void movePlayer(const ClientID& clientID, const float &x, const float &y, const float & angle);
   void removePlayer(const ClientID& clientID);

   ClientPlayer* getPlayer(const ClientID& clientID);

   void drawAllPlayers(Window &window);
   void updateAllPlayers(const sf::Time& time);

private:
   std::unordered_map<ClientID, ClientPlayer*> players;
};

