#pragma once
#include "BaseState.h"
#include "StateManager.h"
#include "Client.h"
#include "ClientPlayersManager.h"
#include <queue>


class GameState : public BaseState
{
public:
   GameState(StateManager *stateManager);
   ~GameState();

   void onCreate() override;
   void onDestroy() override;

   void draw() override;
   void update(const sf::Time &time) override;

   void activate() override;
   void deactivate() override;

   void setServer(const sf::IpAddress & ip, const PortNumber & portNumber);
   bool connect();
   
   void moveToMainMenu(EventDetails* details);
   void clientHandler(const PacketID &id, sf::Packet &packet, Client *client);
private:

   void shoot(EventDetails *details);
   void movePlayer(EventDetails *details);
   void updateConnection(Client *client);

   float sendEventToServerTimer;

   std::queue<MoveDirection> lastDirections;
   ClientPlayersManager playersManager;
   ClientPlayer* player;
   Client client;
};

